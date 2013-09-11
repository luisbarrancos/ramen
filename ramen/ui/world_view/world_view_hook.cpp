// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/world_view/world_view_hook.hpp>

#include<memory>

#include<boost/container/flat_map.hpp>
#include<boost/ptr_container/ptr_map.hpp>

#include<ramen/core/memory.hpp>
#include<ramen/core/exceptions.hpp>

#include<QPen>
#include<QBrush>

#include<ramen/nodes/node.hpp>

#include<ramen/ui/world_view/world_view.hpp>

namespace ramen
{
namespace ui
{
namespace
{

boost::container::flat_map<core::name_t, world_view_hook_t::create_hook_fun> g_factory;
boost::ptr_map<core::name_t, world_view_hook_t> g_hooks;
core::auto_ptr_t<world_view_hook_t> g_default_hook;

} // unnamed

world_view_hook_t::world_view_hook_t() {}
world_view_hook_t::~world_view_hook_t() {}

math::point2f_t world_view_hook_t::input_location( const node_t& n, std::size_t index) const
{
    return math::point2f_t( 0, 0);
}

math::point2f_t world_view_hook_t::output_location( const node_t& n, std::size_t index) const
{
    return math::point2f_t( 0, 0);
}

void world_view_hook_t::draw_edges( const node_t& n, world_view_t& view) const
{
    //for( int i = 0, e = n.num_inputs(); i != e; ++i)
    {
        //const params::input_param_t& in( n.input_param( i));
    }

    //for( int i = 0, e = n.num_outputs(); i != e; ++i)
    {
        //const params::output_param_t& out( n.output_param( i));
    }
}

void world_view_hook_t::draw_node( const node_t& n, world_view_t& view) const
{
    /*
    QPen pen;
    QBrush brush;

    brush.setStyle( Qt::SolidPattern);
    brush.setColor( QColor( 0, 0, 0));

    view.painter().setPen( Qt::NoPen);
    view.painter().setBrush( brush);
    view.painter().drawRoundedRect( QRectF( n.location().x + 3,
                                            n.location().y + 3,
                                            90, 20), 4, 4);

    brush.setColor( QColor( 150, 150, 150));
    view.painter().setBrush( brush);
    view.painter().drawRoundedRect( QRectF( n.location().x,
                                            n.location().y,
                                            90, 20), 4, 4);

    view.painter().setBrush( Qt::NoBrush);

    pen.setColor( QColor( 200, 200, 200));
    view.painter().setPen( pen);
    view.painter().drawRoundedRect( QRectF( n.location().x,
                                            n.location().y,
                                            90, 20), 4, 4);

    pen.setColor( QColor( 255, 255, 255));
    view.painter().setPen( pen);
    view.painter().drawText( QRectF( n.location().x,
                                     n.location().y + 3,
                                     90, 20 - 3),
                                     Qt::AlignCenter,
                                     QString( n.name().c_str()));
    */
}

void world_view_hook_t::init()
{
    RAMEN_ASSERT( g_default_hook.get() == 0);

    g_default_hook.reset( new world_view_hook_t());
}

void world_view_hook_t::register_hook( const core::name_t& id, world_view_hook_t*(*create_hook)())
{
    g_factory[id] = create_hook;
}

world_view_hook_t& world_view_hook_t::hook_for_node( const core::name_t& id)
{
    RAMEN_ASSERT( g_default_hook.get());

    boost::ptr_map<core::name_t, world_view_hook_t>::iterator it( g_hooks.find( id));

    if( it != g_hooks.end())
        return *it->second;

    if( g_factory.find( id) != g_factory.end())
    {
        world_view_hook_t *w = g_factory[id]();
        g_hooks.insert( id, std::auto_ptr<world_view_hook_t>( w));
        return *w;
    }

    return *g_default_hook.get();
}

world_view_hook_t& world_view_hook_t::hook_for_node( const node_t& n)
{
    return hook_for_node( n.node_info()->id);
}

} // ui
} // ramen
