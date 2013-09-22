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

#include<ramen/app/application.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/memory/manager.hpp>

#include<ramen/depgraph/dgraph.hpp>

#include<ramen/nodes/world_node.hpp>

#include<ramen/anim/track.hpp>

#include<ramen/string_algo/valid_c_identifier.hpp>

#include<ramen/util/flags.hpp>

#include<ramen/ui/palette.hpp>

namespace ramen
{
namespace nodes
{

boost::signals2::signal<void ( node_t*)> node_t::node_deleted;
boost::signals2::signal<void ( node_t*,
                               const core::string8_t& old_name,
                               const core::string8_t& new_name)> node_t::node_renamed;

node_t::node_t() : manipulable_t(), parent_( 0), flags_( 0), dont_persist_params_( false)
{
    params_.set_node( this);
}

node_t::node_t( const node_t& other) :  manipulable_t( other),
                                        params_( other.params_)
{
    params_.set_node( this);
    name_ = other.name_;
    flags_ = other.flags_;
    loc_ = other.loc_;
    parent_ = 0;
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

const world_node_t *node_t::world_node() const
{
    node_t& self = const_cast<node_t&>( *this);
    return self.world_node();
}

world_node_t *node_t::world_node()
{
    node_t *p = parent();

    while( p)
    {
        if( world_node_t *c = dynamic_cast<world_node_t*>( p))
            return c;

        p = p->parent();
    }

    return 0;
}

bool node_t::accept_connection( node_t *src, int port) const { return true;}

void node_t::connected( node_t *src, int port)
{
    do_connected( src, port);
}

void node_t::do_connected( node_t *src, int port) {}

void node_t::reconnect_node()
{
    /*
    world_node_t *world = world_node();

    if( world)
    {
        BOOST_FOREACH( edge_t& e, comp->edges())
        {
            if( e.dst == this)
                input_plugs()[ e.port].set_input( e.src);
        }
    }
    */
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

void node_t::add_dependency( params::param_t *src, params::param_t *dst)
{
    //RAMEN_ASSERT( src->is_output());
    //RAMEN_ASSERT( dst->is_input());

    //app().dependency_graph().add_dependency( depgraph::dependency_t( src, dst));
    throw core::not_implemented();
}

void node_t::add_dependency( const core::name_t& src_id, const core::name_t& dst_id)
{
    add_dependency( &param( src_id), &param( dst_id));
}

void node_t::for_each_param( const boost::function<void ( params::param_t*)>& f)
{
    param_set().for_each_param( f);
}

void node_t::param_edit_finished()
{
}

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

// ui
const char *node_t::help_string() const
{
    RAMEN_ASSERT( node_info());

    return node_info()->help;
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

} // nodes
} // ramen
