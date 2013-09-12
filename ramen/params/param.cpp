// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/param.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/nodes/node.hpp>

#include<ramen/params/param_set.hpp>

#include<ramen/string_algo/valid_c_identifier.hpp>

namespace ramen
{
namespace params
{

param_t::param_t() : depgraph::dnode_t(), parent_( 0), flags_( 0)
{
}

param_t::param_t( const param_t& other) :   depgraph::dnode_t( other),
                                            parent_( 0),
                                            id_( other.id_),
                                            ui_label_( other.ui_label_),
                                            flags_( other.flags_)
{
}

void param_t::init()
{
    do_init();
}

void param_t::do_init() {}

void param_t::set_parent( param_t *parent)
{
    parent_ = parent;
}

const param_set_t *param_t::param_set() const
{
    param_t& self = const_cast<param_t&>( *this);
    return self.param_set();
}

param_set_t *param_t::param_set()
{
    param_t *p = parent();
    while( p)
    {
        if( param_set_t *ps = dynamic_cast<param_set_t*>( p))
            return ps;

        p = p->parent();
    }

    return 0;
}

const node_t *param_t::node() const
{
    RAMEN_ASSERT( param_set());

    return param_set()->node();
}

node_t *param_t::node()
{
    RAMEN_ASSERT( param_set());

    return param_set()->node();
}

const world_node_t *param_t::world_node() const
{
    RAMEN_ASSERT( param_set());

    const node_t *n = node();

    if( n)
        return n->world_node();

    return 0;
}

world_node_t *param_t::world_node()
{
    RAMEN_ASSERT( param_set());

    node_t *n = node();

    if( n)
        return n->world_node();

    return 0;
}

void param_t::set_ui_label( core::string8_t name)
{
    ui_label_ = name;
}

void param_t::set_id( const core::name_t& identifier)
{
    if( !string_algo::is_valid_c_identifier( identifier.c_str()))
        throw core::runtime_error( "Invalid id for param_t");

    id_ = identifier;
}

void param_t::emit_param_changed( change_reason reason)
{
    if( reason != silent_edit)
    {
        if( param_set_t *p = param_set())
            p->param_changed( this, reason);
    }
}

void param_t::create_tracks( anim::track_t *parent)
{
    //if( !is_static())
        do_create_tracks( parent);
}

void param_t::do_create_tracks( anim::track_t *parent) {}

void param_t::set_frame( float frame)
{
    evaluate( frame);
    do_set_frame( frame);
    emit_param_changed( time_changed);
}

void param_t::do_set_frame( float frame) {}

void param_t::evaluate( float frame)	{ do_evaluate( frame);}
void param_t::do_evaluate( float frame)	{}

// undo
core::auto_ptr_t<undo::command_t> param_t::create_command() { return do_create_command();}

core::auto_ptr_t<undo::command_t> param_t::do_create_command()
{
    return core::auto_ptr_t<undo::command_t>();
}

// paths
void param_t::convert_relative_paths( const boost::filesystem::path& old_base,
                                      const boost::filesystem::path& new_base)
{
    do_convert_relative_paths( old_base, new_base);
}

void param_t::do_convert_relative_paths( const boost::filesystem::path& old_base,
                                         const boost::filesystem::path& new_base)
{
}

void param_t::make_paths_absolute()     { do_make_paths_absolute();}
void param_t::do_make_paths_absolute()  {}

void param_t::make_paths_relative()     { do_make_paths_relative();}
void param_t::do_make_paths_relative()  {}

// util
void param_t::apply_function( const boost::function<void ( param_t*)>& f)
{
    f( this);
    do_apply_function( f);
}

void param_t::do_apply_function( const boost::function<void ( param_t*)>& f) {}

param_t *new_clone( const param_t& other) { return other.clone();}

} // params
} // ramen
