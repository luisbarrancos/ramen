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
#include<ramen/app/composition.hpp>

#include<ramen/memory/manager.hpp>

#include<ramen/image/color_bars.hpp>

#include<ramen/anim/track.hpp>

#include<ramen/util/string.hpp>
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

node_t::node_t() : manipulable_t(), flags_( 0), composition_( 0), dont_persist_params_( false)
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
    composition_ = other.composition_;
    dont_persist_params_ = other.dont_persist_params_;
}

node_t::~node_t()
{
    deleted( this);
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
    format_changed();
}

void node_t::set_name( const std::string& n)
{
    RAMEN_ASSERT( util::is_string_valid_identifier( n));

    if( !util::is_string_valid_identifier( n))
        throw std::runtime_error( "Invalid name for node");

    name_ = n;
}

const composition_t *node_t::composition() const
{
    return composition_;
}

composition_t *node_t::composition()
{
    return composition_;
}

void node_t::set_composition( composition_t *comp)
{
    composition_ = comp;
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
    if( const composition_t *c = composition())
        return c->autokey();

    return false;
}

bool node_t::track_mouse() const { return true;}

// inputs

int node_t::find_input( const core::name_t& id) const
{
    int index = 0;
    BOOST_FOREACH( const node_input_plug_t& i, input_plugs())
    {
        if( i.id() == id)
            return index;

        ++index;
    }

    return -1;
}

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

void node_t::add_input_plug( const std::string &id,
                             bool optional,
                             const Imath::Color3c &color,
                             const std::string &tooltip)
{
    inputs_.push_back( node_input_plug_t( id, optional, color, tooltip));
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

const node_t *node_t::output( std::size_t i) const
{
    RAMEN_ASSERT( has_output_plug());
    RAMEN_ASSERT( i < num_outputs());
    return boost::get<0>( outputs_[0].connections()[i]);
}

node_t *node_t::output( std::size_t i)
{
    RAMEN_ASSERT( has_output_plug());
    RAMEN_ASSERT( i < num_outputs());
    return boost::get<0>( outputs_[0].connections()[i]);
}

void node_t::add_output_plug( const std::string &id,
                              const Imath::Color3c& color,
                              const std::string& tooltip)
{
    RAMEN_ASSERT( !has_output_plug());
    outputs_.push_back( new node_output_plug_t( this, id, color, tooltip));
}

void node_t::add_output_plug()
{
    add_output_plug( "output", ui::palette_t::instance().color( "out plug"), "output" );
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
    composition_t *comp = composition();

    if( comp)
    {
        BOOST_FOREACH( edge_t& e, comp->edges())
        {
            if( e.dst == this)
                input_plugs()[ e.port].set_input( e.src, core::name_t( "unused"));
        }
    }
}

// params
void node_t::create_params()
{
    do_create_params();
    boost::range::for_each( param_set(), boost::bind( &param_t::init, _1));
}

void node_t::do_create_params() {}

const param_t& node_t::param( const std::string& identifier) const
{
    return param_set().find( identifier);
}

param_t& node_t::param( const std::string& identifier)
{
    return param_set().find( identifier);
}

void node_t::for_each_param( const boost::function<void ( param_t*)>& f)
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
    RAMEN_ASSERT( composition());

    // keep the format up to date
    Imath::Box2i old_format( full_format());
    float old_aspect = aspect_ratio();
    Imath::V2f old_proxy_scale = proxy_scale();

    render::context_t context = composition()->current_context();
    context.subsample = 1;
    calc_format( context);

    if( old_format != format() || old_aspect != aspect_ratio() || old_proxy_scale != proxy_scale())
        format_changed();

    changed( this);
    set_notify_dirty( false);
}

void node_t::calc_frames_needed( const render::context_t& context)
{
    do_calc_frames_needed( context);

    if( !frames_needed().empty())
        std::sort( frames_needed().begin(), frames_needed().end(), frames_needed_less());
}

void node_t::do_calc_frames_needed( const render::context_t& context) {}

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

// hash
void node_t::clear_hash()
{
    hash_generator().reset();

    set_cacheable( !is_frame_varying());

    if( cacheable())
    {
        BOOST_FOREACH( node_input_plug_t& i, input_plugs())
        {
            if( i.connected())
            {
                if( !i.input_node()->cacheable())
                {
                    set_cacheable( false);
                    return;
                }
            }
        }
    }

    frames_needed().clear();
}

void node_t::calc_hash_str( const render::context_t& context)
{
    RAMEN_ASSERT( hash_generator().empty());
    RAMEN_ASSERT( !hash_generator().finalized());

    if( !cacheable())
        return;

    bool valid = is_valid();

    if( valid && is_identity())
    {
        if( num_inputs() > 0 && input())
            hash_generator() << input()->hash_str();

        return;
    }

    hash_generator() << typeid( *this).name();

    if( !valid)
    {
        add_context_to_hash_string( context);
        return;
    }

    // handle the case when all inputs are optional and disconnected.
    if( num_inputs() != 0)
    {
        bool all_optional = true;
        bool not_connected = true;

        for( int i = 0; i < num_inputs(); ++i)
        {
            if( !input_plugs()[i].optional())
                all_optional = false;

            if( input(i))
                not_connected = false;
        }

        if( all_optional && not_connected)
            add_context_to_hash_string( context);
    }

    // for each needed input frame...
    calc_frames_needed( context);

    if( !frames_needed_.empty() && !ignored())
        add_needed_frames_to_hash( context);
    else
    {
        for( int i = 0; i < num_inputs(); ++i)
            if( input( i) && include_input_in_hash( i))
                hash_generator() << i << input( i)->hash_str();
    }

    do_calc_hash_str( context);
}

void node_t::do_calc_hash_str( const render::context_t& context)
{
    param_set().add_to_hash( hash_generator());
}

std::string node_t::hash_str() const { return hash_generator().str();}

const hash::generator_t::digest_type& node_t::digest() { return hash_generator().digest();}

void node_t::add_context_to_hash_string( const render::context_t& context)
{
    hash_generator() << context.subsample;
    hash_generator() << context.motion_blur_extra_samples;
    hash_generator() << context.motion_blur_shutter_factor;
}

bool node_t::include_input_in_hash( int num) const { return true;}

void node_t::add_needed_frames_to_hash( const render::context_t& context)
{
    const_frames_needed_iterator it( frames_needed().begin()), end( frames_needed().end());

    while( it != end)
    {
        float cur_frame = it->first;
        render::context_t new_context( context);
        new_context.frame = cur_frame;

        while( 1)
        {
            node_t *n = input( it->second);
            depth_first_inputs_search( *n, boost::bind( &node_t::set_frame, _1, cur_frame));
            depth_first_inputs_search( *n, boost::bind( &node_t::clear_hash, _1));
            depth_first_inputs_search( *n, boost::bind( &node_t::calc_hash_str, _1, new_context));
            hash_generator() << it->second << n->hash_str();
            ++it;

            if( it == end || it->first != cur_frame)
                break;
        }
    }

    // restore original time here
    for( int i = 0; i < num_inputs(); ++i)
    {
        if( node_t *n = input( i))
        {
            depth_first_inputs_search( *n, boost::bind( &node_t::set_frame, _1, context.frame));
            depth_first_inputs_search( *n, boost::bind( &node_t::clear_hash, _1));
            depth_first_inputs_search( *n, boost::bind( &node_t::calc_hash_str, _1, context));
        }
    }
}

// cache
bool node_t::is_frame_varying() const { return false;}

void node_t::create_tracks( anim::track_t *root)
{
    std::auto_ptr<anim::track_t> top( new anim::track_t( name()));

    BOOST_FOREACH( param_t& p, param_set())
    {
        p.create_tracks( top.get());
    }

    do_create_tracks( top.get());
    root->add_child( top);
}

void node_t::set_frame( float f)
{
    boost::range::for_each( param_set(), boost::bind( &param_t::set_frame, _1, f));
    do_set_frame( f);
}

void node_t::evaluate_params( float frame)
{
    boost::range::for_each( param_set(), boost::bind( &param_t::evaluate, _1, frame));
}

// ui
const char *node_t::help_string() const
{
    RAMEN_ASSERT( metaclass());

    return metaclass()->help;
}

void node_t::update_widgets()
{
    boost::range::for_each( param_set(), boost::bind( &param_t::update_widgets, _1));
    do_update_widgets();
}

void node_t::convert_relative_paths( const boost::filesystem::path& old_base,
                                     const boost::filesystem::path& new_base)
{
    boost::range::for_each( param_set(),
                            boost::bind( &param_t::convert_relative_paths,
                                         _1, old_base, new_base));
}

void node_t::make_paths_absolute()
{
    boost::range::for_each( param_set(),
                            boost::bind( &param_t::make_paths_absolute, _1));
}

void node_t::make_paths_relative()
{
    boost::range::for_each( param_set(), boost::bind( &param_t::make_paths_relative, _1));
}

// serialization
/*
void node_t::read(const serialization::yaml_node_t& in, const std::pair<int,int>& version)
{
    std::string n;
    in.get_value( "name", n);

    RAMEN_ASSERT( util::is_string_valid_identifier( n));

    if( !util::is_string_valid_identifier( n))
        throw std::runtime_error( "Bad name in node_t");

    set_name( n);

    if( !in.get_optional_value( "comp_pos", loc_))
        set_autolayout( true);

    // create needed extra inputs if needed.
    if( variable_num_inputs())
    {
        int num_ins = num_inputs();
        in.get_optional_value( "num_inputs", num_ins);

        while( num_ins != num_inputs())
            add_new_input_plug();
    }

    bool flag = false;
    if( in.get_optional_value( "ignored", flag))
        set_ignored( flag);

    serialization::yaml_node_t prms( in.get_node( "params"));
    param_set().read( prms);

    do_read( in, version);
}

void node_t::do_read( const serialization::yaml_node_t& in,
                      const std::pair<int,int>& version)
{
}

void node_t::write( serialization::yaml_oarchive_t& out) const
{
    RAMEN_ASSERT( metaclass() && "Trying to serialize an abstract node");
    out.begin_map();
    write_node_info( out);
    param_set().write( out);
    do_write( out);
    out.end_map();
}

void node_t::do_write( serialization::yaml_oarchive_t& out) const {}

void node_t::write_node_info( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "class" << YAML::Value;
    out.flow();
    out.begin_seq();
    out << metaclass()->id
        << metaclass()->major_version << metaclass()->minor_version;
    out.end_seq();

    out << YAML::Key << "name"  << YAML::DoubleQuoted << YAML::Value << name();
    out << YAML::Key << "comp_pos" << YAML::Value << location();

    if( ignored())
        out << YAML::Key << "ignored" << YAML::Value << true;

    if( variable_num_inputs())
        out << YAML::Key << "num_inputs" << YAML::Value << num_inputs();
}
*/

void node_t::format_changed()
{
    boost::range::for_each( param_set(),
                            boost::bind( &param_t::format_changed, _1, format(),
                                         aspect_ratio(), proxy_scale()));
}

void node_t::calc_format( const render::context_t& context)
{
    is_valid_ = is_valid();
    is_identity_ = false;

    if( is_valid_)
        is_identity_ = is_identity();

    // init with invalid values, to catch the case
    // when we forget to set them.
    aspect_ = 0.0f;
    proxy_scale_ =  Imath::V2f( 0, 0);

    if( is_valid_ && !is_identity_)
        do_calc_format( context);
    else
        node_t::do_calc_format( context);

    // save the full resolution format.
    full_format_ = format_;

    // make sure the values have been initialized
    RAMEN_ASSERT( aspect_ratio() != 0.0f);
    RAMEN_ASSERT( proxy_scale().x != 0.0f && proxy_scale().y != 0.0f);
}

void node_t::do_calc_format( const render::context_t& context)
{
    if( ( num_inputs() != 0) && input())
    {
        node_t *in = input();
        set_format( in->format());
        set_aspect_ratio( in->aspect_ratio());
        set_proxy_scale( in->proxy_scale());
    }
    else
    {
        // init with default values
        set_format( Imath::Box2i( Imath::V2i( 0, 0), Imath::V2i( context.default_format.area().max.x - 1,
                                                                 context.default_format.area().max.y - 1)));
        set_aspect_ratio( context.default_format.aspect);
        set_proxy_scale( Imath::V2f( 1.0f, 1.0f));
    }
}

void node_t::set_aspect_ratio( float a)
{
    RAMEN_ASSERT( a > 0);
    aspect_ = a;
}

void node_t::set_proxy_scale( const Imath::V2f& s)
{
    RAMEN_ASSERT( s.x > 0 && s.y > 0);
    proxy_scale_ = s;
}

void node_t::set_bounds( const Imath::Box2i& bounds) { bounds_ = bounds;}

void node_t::calc_bounds( const render::context_t& context)
{
    if( is_valid_ && !is_identity_)
        do_calc_bounds( context);
    else
        node_t::do_calc_bounds( context);
}

void node_t::do_calc_bounds( const render::context_t& context)
{
    if( ( num_inputs() != 0) && input())
        set_bounds( input()->bounds());
    else
        set_bounds( format());
}

void node_t::clear_interest() { interest_ = Imath::Box2i();}

void node_t::set_interest( const Imath::Box2i& roi) { interest_ = roi;}
void node_t::add_interest( const Imath::Box2i& roi) { interest_.extendBy( roi);}

void node_t::calc_inputs_interest( const render::context_t& context)
{
    if( !is_valid_)
        return;

    if( !is_identity_)
        do_calc_inputs_interest( context);
    else
        node_t::do_calc_inputs_interest( context);
}

void node_t::do_calc_inputs_interest( const render::context_t& context)
{
    // by default, pass the interest area to the inputs
    BOOST_FOREACH( node_input_plug_t& i, input_plugs())
    {
        if( i.connected())
        {
            if( node_t *in = dynamic_cast<node_t*>( i.input_node()))
                in->add_interest( interest_);
        }
    }
}

void node_t::set_defined( const Imath::Box2i& b) { defined_ = b;}

void node_t::calc_defined( const render::context_t& context)
{
    if( !is_valid_)
        defined_ = ImathExt::intersect( format_, interest_);
    else
    {
        if( is_identity_)
        {
            if( num_inputs() != 0 && input())
                set_defined( input()->defined());
            else
            {
                // this should never happen.
                RAMEN_ASSERT( 0 && "calc_defined: is_identity == true but first input is 0");
            }
        }
        else
            do_calc_defined( context);
    }

    // limit the image size
    if( defined_.size().x > app().preferences().max_image_width())
        defined_.max.x = defined_.min.x + app().preferences().max_image_width();

    if( defined_.size().y > app().preferences().max_image_height())
        defined_.max.y = defined_.min.y + app().preferences().max_image_height();
}

void node_t::do_calc_defined( const render::context_t& context)
{
    defined_ = ImathExt::intersect( bounds_, interest_);
}

void node_t::subsample_areas( const render::context_t& context)
{
    if( context.subsample != 1)
    {
        format_   = ImathExt::scale( format_  , 1.0f / context.subsample);
        bounds_   = ImathExt::scale( bounds_  , 1.0f / context.subsample);
        interest_ = ImathExt::scale( interest_, 1.0f / context.subsample);
        defined_  = ImathExt::scale( defined_ , 1.0f / context.subsample);
    }
}

void node_t::recursive_calc_format( const render::context_t& context)
{
    depth_first_inputs_search( *this, boost::bind( &node_t::calc_format_fun, _1, context));
}

bool node_t::use_cache( const render::context_t& context) const { return true;}

bool node_t::read_image_from_cache( const render::context_t& context)
{
    if( !cacheable() || !use_cache( context))
    {
        #ifndef NDEBUG
            std::cout << "cache miss, node " << name() << " not cacheable\n" << std::endl;
        #endif

        return false;
    }

    boost::optional<image::buffer_t> cached( app().memory_manager().find_in_cache( digest(), defined()));

    if( cached)
    {
        image_ = cached.get();
        return true;
    }

    #ifndef NDEBUG
        std::cout << "cache miss: " << name() << ", " << hash_generator().digest_as_string()
                  << "\n" << std::endl;
    #endif

    return false;
}

void node_t::write_image_to_cache( const render::context_t& context)
{
    if( !cacheable() || !use_cache( context))
        return;

    if( !defined().isEmpty())
        app().memory_manager().insert_in_cache( this, digest(), image_);
}

// images
void node_t::alloc_image()
{
    image_ = image::buffer_t( defined(), 4);

    if( !image_.empty())
        boost::gil::fill_pixels( image_view(), image::pixel_t( 0, 0, 0, 0));
}

void node_t::release_image()	{ image_ = image::buffer_t();}

image::image_view_t node_t::image_view() { return image_.rgba_subimage_view( defined());}

image::const_image_view_t node_t::const_image_view() const { return image_.const_rgba_subimage_view( defined());}

image::image_view_t node_t::subimage_view( int x, int y, int w, int h)
{
    return image_.rgba_subimage_view( Imath::Box2i( Imath::V2i( x, y), Imath::V2i( w - 1, h - 1)));
}

image::image_view_t node_t::subimage_view( const Imath::Box2i& area)
{
    return image_.rgba_subimage_view( area);
}

image::const_image_view_t node_t::const_subimage_view( int x, int y, int w, int h) const
{
    return image_.const_rgba_subimage_view( Imath::Box2i( Imath::V2i( x, y), Imath::V2i( w - 1, h - 1)));
}

image::const_image_view_t node_t::const_subimage_view( const Imath::Box2i& area) const
{
    return image_.const_rgba_subimage_view( area);
}

void node_t::recursive_process( const render::context_t& context)
{
    if( interacting())
    {
        RAMEN_ASSERT( context.mode == render::interface_render);
    }

    if( !is_valid())
    {
        alloc_image();
        image::make_color_bars( image_view());
        return;
    }

    if( is_identity())
    {
        // TODO: Not sure if this is correct, check it.
        if( node_t *in = input())
        {
            in->recursive_process( context);
            image_ = in->image_;
            in->release_image();
        }

        return;
    }

    if( read_image_from_cache( context))
        return;

    do_recursive_process( context);

    if( !context.render_cancelled())
        write_image_to_cache( context);

    BOOST_FOREACH( node_input_plug_t& i, input_plugs())
    {
        if( i.connected())
        {
            if( node_t *in = dynamic_cast<node_t*>( i.input_node()))
                in->release_image();
        }
    }
}

void node_t::do_recursive_process( const render::context_t& context)
{
    // in this case, you have to manually process the inputs.
    if( !frames_needed().empty())
    {
        if( !context.render_cancelled())
        {
            alloc_image();
            process( context);
        }

        return;
    }

    // normal case

    // pixels are shared between image buffers.
    // as long as we keep a copy, the pixels won't be deleted
    std::vector<image::buffer_t> buffers;

    BOOST_FOREACH( node_input_plug_t& i, input_plugs())
    {
        if( context.render_cancelled())
            return;

        if( i.connected())
        {
            if( node_t *in = dynamic_cast<node_t*>( i.input_node()))
            {
                if( in->image_empty())
                    in->recursive_process( context);

                buffers.push_back( in->image());
            }
        }
    }

    int j = 0;
    BOOST_FOREACH( node_input_plug_t& i, input_plugs())
    {
        if( i.connected())
        {
            if( node_t *in = dynamic_cast<node_t*>( i.input_node()))
            {
                if( in->image_empty())
                    in->set_image( buffers[j]);

                ++j;
            }
        }
    }

    if( !context.render_cancelled())
    {
        alloc_image();
        process( context);
    }
}

void node_t::process( const render::context_t& context)
{
    if( context.render_cancelled())
        return;

    if( !defined().isEmpty())
        do_process( context);
}

void node_t::do_process( const render::context_t& context) {}

// used in renderer and other places to recursive calc areas
void node_t::calc_format_fun( node_t& n, const render::context_t& context)
{
    if( node_t *in = dynamic_cast<node_t*>( &n))
        in->calc_format( context);
}

void node_t::calc_bounds_fun( node_t& n, const render::context_t& context)
{
    if( node_t *in = dynamic_cast<node_t*>( &n))
        in->calc_bounds( context);
}

void node_t::clear_interest_fun( node_t& n)
{
    if( node_t *in = dynamic_cast<node_t*>( &n))
        in->clear_interest();
}

void node_t::calc_inputs_interest_fun( node_t& n, const render::context_t& context)
{
    if( node_t *in = dynamic_cast<node_t*>( &n))
        in->calc_inputs_interest( context);
}

void node_t::calc_defined_fun( node_t& n, const render::context_t& context)
{
    if( node_t *in = dynamic_cast<node_t*>( &n))
        in->calc_defined( context);
}

void node_t::subsample_areas_fun( node_t& n, const render::context_t& context)
{
    if( node_t *in = dynamic_cast<node_t*>( &n))
        in->subsample_areas( context);
}

node_t *new_clone( const node_t& other)
{
    node_t *n = other.clone();
    n->cloned();
    return n;
}

} // ramen
