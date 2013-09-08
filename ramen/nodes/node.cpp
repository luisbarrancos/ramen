// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/node.hpp>

#include<vector>
#include<stdexcept>
#include<algorithm>
#include<iostream>

#include<boost/bind.hpp>
#include<boost/foreach.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/nodes/graph_algorithm.hpp>

#include<ramen/app/application.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/memory/manager.hpp>

#include<ramen/nodes/composition_node.hpp>

#include<ramen/anim/track.hpp>

#include<ramen/string_algo/valid_c_identifier.hpp>

#include<ramen/util/flags.hpp>

#include<ramen/ui/palette.hpp>

namespace ramen
{
namespace
{

struct frames_needed_less
{
    bool operator()( const std::pair<int, int>& a, const std::pair<int, int>& b) const
    {
        if( a.first < b.first)
            return true;

        if( a.first == b.first)
            return a.second < b.second;

        return false;
    }
};

} // unnamed

boost::signals2::signal<void ( node_t*)> node_t::node_deleted;
boost::signals2::signal<void ( node_t*,
                               const core::string8_t& old_name,
                               const core::string8_t& new_name)> node_t::node_renamed;

node_t::node_t() : manipulable_t(), parent_( 0), flags_( 0), dont_persist_params_( false)
{
    params_.set_node( this);
}

node_t::node_t( const node_t& other) :  manipulable_t( other),
                                        params_( other.params_),
                                        inputs_( other.inputs_),
                                        outputs_( other.outputs_)
{
    params_.set_node( this);
    name_ = other.name_;
    boost::range::for_each( outputs_, boost::bind( &node_output_plug_t::set_parent_node, _1, this));
    flags_ = other.flags_;
    loc_ = other.loc_;
    parent_ = 0;
    dont_persist_params_ = other.dont_persist_params_;
}

node_t::~node_t()
{
    node_deleted( this);
}

node_t *node_t::clone() const
{
    return do_clone();
}

void node_t::cloned()
{
    for( int i = 0; i < num_inputs(); ++i)
        connected( 0, i);

    create_manipulators();
}

void node_t::set_name( core::string8_t n)
{
    if( !string_algo::is_valid_c_identifier( n))
        throw core::runtime_error( "Invalid name for node");

    name_.swap( n);
    node_renamed( this, n, name_);
}

const composite_node_t *node_t::parent() const
{
    return parent_;
}

composite_node_t *node_t::parent()
{
    return parent_;
}

void node_t::set_parent( composite_node_t *comp)
{
    parent_ = comp;
}

const composition_node_t *node_t::composition_node() const
{
    node_t& self = const_cast<node_t&>( *this);
    return self.composition_node();
}

composition_node_t *node_t::composition_node()
{
    node_t *p = parent();

    while( p)
    {
        if( composition_node_t *c = dynamic_cast<composition_node_t*>( p))
            return c;

        p = p->parent();
    }

    return 0;
}

// visitor
void node_t::accept( node_visitor& v) { v.visit( this);}

bool node_t::selected() const { return flags_ & selected_bit;}

void node_t::select( bool state)
{
    if( state)
    {
        if( ui_invisible())
        {
            // ui invisible nodes can't be selected
            flags_ &= ~selected_bit;
        }
        else
            flags_ |= selected_bit;
    }
    else
        flags_ &= ~selected_bit;
}

void node_t::toggle_selection()	{ select( !selected());}

bool node_t::ignored() const        { return util::test_flag( flags_, ignored_bit);}
void node_t::set_ignored( bool b)   { util::set_flag( flags_, ignored_bit, b);}

bool node_t::plugin_error() const       { return util::test_flag( flags_, plugin_error_bit);}
void node_t::set_plugin_error( bool b)  { util::set_flag( flags_, plugin_error_bit, b);}

bool node_t::autolayout() const         { return util::test_flag( flags_, autolayout_bit);}
void node_t::set_autolayout( bool b)    { util::set_flag( flags_, autolayout_bit, b);}

bool node_t::cacheable() const          { return flags_ & cacheable_bit;}
void node_t::set_cacheable( bool b)     { util::set_flag( flags_, cacheable_bit, b);}

bool node_t::notify_dirty() const       { return util::test_flag( flags_, notify_dirty_bit);}
void node_t::set_notify_dirty( bool b)  { util::set_flag( flags_, notify_dirty_bit, b);}

bool node_t::ui_invisible() const       { return flags_ & ui_invisible_bit;}
void node_t::set_ui_invisible( bool b)  { util::set_flag( flags_, ui_invisible_bit, b);}

bool node_t::is_active() const     { return util::test_flag( flags_, active_bit);}
bool node_t::is_context() const    { return util::test_flag( flags_, context_bit);}

bool node_t::autokey() const
{
    if( const composition_node_t *c = composition_node())
        return c->autokey();

    return false;
}

bool node_t::track_mouse() const { return true;}

// inputs

const node_t *node_t::input( std::size_t i) const
{
    RAMEN_ASSERT( i < inputs_.size());
    return inputs_[i].input_node();
}

node_t *node_t::input( std::size_t i)
{
    RAMEN_ASSERT( i < inputs_.size());
    return inputs_[i].input_node();
}

void node_t::add_input_plug( const core::string8_t& ui_label,
                             const color::color3c_t& color,
                             bool optional)
{
    inputs_.push_back( node_input_plug_t( ui_label, color, optional));
}

std::size_t node_t::num_outputs() const
{
    if( !has_output_plug())
        return 0;

    return outputs_[0].connections().size();
}

const node_output_plug_t& node_t::output_plug() const
{
    RAMEN_ASSERT( has_output_plug());
    return outputs_[0];
}

node_output_plug_t& node_t::output_plug()
{
    RAMEN_ASSERT( has_output_plug());
    return outputs_[0];
}

void node_t::add_output_plug( const core::string8_t& ui_label,
                              const color::color3c_t& color)
{
    RAMEN_ASSERT( !has_output_plug());
    outputs_.push_back( new node_output_plug_t( this, ui_label, color));
}

void node_t::add_output_plug()
{
    add_output_plug( "output", ui::palette_t::instance().color( "out plug"));
}

bool node_t::accept_connection( node_t *src, int port) const { return true;}

void node_t::connected( node_t *src, int port)
{
    if( variable_num_inputs() && src != 0 && port == num_inputs()-1)
        add_new_input_plug();

    do_connected( src, port);
}

void node_t::do_connected( node_t *src, int port) {}

void node_t::add_new_input_plug()
{
    RAMEN_ASSERT( 0);
    /*
    add_input_plug( input_plug_info_t( ui::palette_t::instance().color("back plug")), true);
    reconnect_node();
    */
}

void node_t::reconnect_node()
{
    composition_node_t *comp = composition_node();

    if( comp)
    {
        BOOST_FOREACH( edge_t& e, comp->edges())
        {
            if( e.dst == this)
                input_plugs()[ e.port].set_input( e.src);
        }
    }
}

// params
void node_t::create_params()
{
    do_create_params();
    boost::range::for_each( param_set(), boost::bind( &params::param_t::init, _1));
}

void node_t::do_create_params() {}

const params::param_t& node_t::param( const core::name_t& identifier) const
{
    if( const params::param_t *p = param_set().find( identifier))
        return *p;

    // TODO: throw something more appropiate here.
    throw core::runtime_error( core::make_string( "param ", identifier.c_str(), " not found"));
}

params::param_t& node_t::param( const core::name_t& identifier)
{
    if( params::param_t *p = param_set().find( identifier))
        return *p;

    // TODO: throw something more appropiate here.
    throw core::runtime_error( core::make_string( "param ", identifier.c_str(), " not found"));
}

void node_t::for_each_param( const boost::function<void ( params::param_t*)>& f)
{
    param_set().for_each_param( f);
}

void node_t::param_edit_finished() { notify();}

void node_t::notify()
{
    breadth_first_outputs_search( *this, boost::bind( &node_t::do_notify, _1));
}

void node_t::do_notify()
{
    RAMEN_ASSERT( false);
}

void node_t::begin_active()
{
    flags_ |= active_bit;
    do_begin_active();
}

void node_t::end_active()
{
    do_end_active();
    flags_ &= ~active_bit;
}

void node_t::begin_context()
{
    flags_ |= context_bit;
    do_begin_context();
}

void node_t::end_context()
{
    do_end_context();
    flags_ &= ~context_bit;
}

bool node_t::interacting() const
{
    return flags_ & interacting_bit;
}

void node_t::begin_interaction()
{
    do_begin_interaction();
    flags_ |= interacting_bit;
}

void node_t::end_interaction()
{
    do_end_interaction();
    flags_ &= ~interacting_bit;
}

bool node_t::is_valid() const
{
    BOOST_FOREACH( const node_input_plug_t& i, input_plugs())
    {
        if( !i.connected() && !i.optional())
            return false;
    }

    if( ignored())
    {
        if( num_inputs() == 0)
            return false;

        // handle the all inputs optional case
        bool all_optional = true;
        bool all_disconnected = true;

        BOOST_FOREACH( const node_input_plug_t& i, input_plugs())
        {
            if( i.connected())
                all_disconnected = false;

            if( !i.optional())
                all_optional = false;
        }

        if( all_optional && all_disconnected)
            return false;
    }

    return do_is_valid();
}

bool node_t::do_is_valid() const { return true;}

bool node_t::is_identity() const
{
    RAMEN_ASSERT( is_valid());

    if( ignored())
        return true;

    // generators can never be identity
    if( num_inputs() == 0)
        return false;

    return do_is_identity();
}

bool node_t::do_is_identity() const { return false;}

// cache
bool node_t::is_frame_varying() const { return false;}

void node_t::create_tracks( anim::track_t *root)
{
    core::auto_ptr_t<anim::track_t> top( new anim::track_t( name()));

    BOOST_FOREACH( params::param_t& p, param_set())
    {
        p.create_tracks( top.get());
    }

    do_create_tracks( top.get());
    root->add_child( boost::move( top));
}

void node_t::set_frame( float f)
{
    boost::range::for_each( param_set(), boost::bind( &params::param_t::set_frame, _1, f));
    do_set_frame( f);
}

void node_t::evaluate_params( float frame)
{
    boost::range::for_each( param_set(), boost::bind( &params::param_t::evaluate, _1, frame));
}

// ui
const char *node_t::help_string() const
{
    RAMEN_ASSERT( node_info());

    return node_info()->help;
}

void node_t::update_widgets()
{
}

void node_t::convert_relative_paths( const boost::filesystem::path& old_base,
                                     const boost::filesystem::path& new_base)
{
    boost::range::for_each( param_set(),
                            boost::bind( &params::param_t::convert_relative_paths,
                                         _1, old_base, new_base));
}

void node_t::make_paths_absolute()
{
    boost::range::for_each( param_set(),
                            boost::bind( &params::param_t::make_paths_absolute, _1));
}

void node_t::make_paths_relative()
{
    boost::range::for_each( param_set(), boost::bind( &params::param_t::make_paths_relative, _1));
}

node_t *new_clone( const node_t& other)
{
    node_t *n = other.clone();
    n->cloned();
    return n;
}

} // ramen
