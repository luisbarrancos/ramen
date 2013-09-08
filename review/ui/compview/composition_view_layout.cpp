// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/compview/composition_view_layout.hpp>

#include<boost/foreach.hpp>

#include<ramen/assert.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/ui/compview/draw_pick_visitors.hpp>

namespace ramen
{
namespace ui
{

composition_view_layout_t::composition_view_layout_t() {}

void composition_view_layout_t::set_world( const math::box2f_t& w)
{
    world_ = w;
}

void composition_view_layout_t::set_interest_point( const math::point2f_t& p)
{
    interest_point_ = p;
}

void composition_view_layout_t::place_node( node_t *n) const
{
    if( world_.is_empty())
    {
        do_place_node( n, math::point2f_t( 0, 0));
        return;
    }

    math::point2f_t p;

    /*
    if( interest_point_ && world_.intersects( *interest_point_))
    {
        float u = math::lerpfactor( interest_point_->x, world_.min.x, world_.max.x);
        float v = math::lerpfactor( interest_point_->y, world_.min.y, world_.max.y);
    }
    else
    */
    {
        float u = 0.5f;
        float v = 0.5f;

        float umargin = world_.size().x / 10;
        float vmargin = world_.size().y / 10;

        p = math::point2f_t( world_.min.x + umargin + ( u * ( world_.size().x - umargin - umargin)),
                             world_.min.y + vmargin + ( v * ( world_.size().y - vmargin - vmargin)));
    }

    do_place_node( n, p);
}

void composition_view_layout_t::place_node_near_node( node_t *n, node_t *other) const
{
    // TODO: this could be much better
    math::point2f_t p( other->location().x, other->location().y);
    p.x += 20;
    p.y += 60;
    do_place_node( n, p);
}

math::box2f_t composition_view_layout_t::get_node_bbox( node_t *n) const
{
    math::box2f_t box( n->location());
    size_node_visitor v;
    n->accept( v);
    box.max.x += v.size.x;
    box.max.y += v.size.y;
    return box;
}

bool composition_view_layout_t::box_intersects_any_node( const math::box2f_t& box) const
{
    BOOST_FOREACH( node_t& n, app().document().composition_node().nodes())
    {
        box_pick_node_visitor visitor( box);
        n.accept( visitor);

        if( visitor.result)
            return true;
    }

    return false;
}

void composition_view_layout_t::do_place_node( node_t *n, const math::point2f_t& p) const
{
    math::point2f_t q( p);

    // init with a known position
    n->set_location( math::point2f_t( 0, 0));
    math::point2f_t size = get_node_bbox( n).max;

    if( box_intersects_any_node( math::box2f_t( q, math::point2f_t( q.x + size.x,
                                                                    q.y + size.y))))
    {
    }

    n->set_location( math::point2f_t( q.x, q.y));
}

} // ui
} // ramen
