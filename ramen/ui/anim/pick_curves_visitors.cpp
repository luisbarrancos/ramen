// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/anim/pick_curves_visitors.hpp>

#include<ramen/anim/float_curve.hpp>
#include<ramen/anim/shape_curve.hpp>
#include<ramen/anim/fwd_diff_cubic_evaluator.hpp>

#include<ramen/ui/anim/anim_curves_view.hpp>

namespace ramen
{
namespace ui
{
namespace
{

bool pick_tangent( const anim_curves_view_t& view,
                   const anim::float_curve_t& c,
                   const anim::float_key_t& k,
                   const anim::float_key_t *prev,
                   const anim::float_key_t *next,
                   const math::point2i_t& p,
                   bool& left)
{
    math::point2i_t q( view.world_to_screen( math::point2f_t( k.time(), c.relative_to_absolute( k.value()))));

    bool pick_left_tan;
    bool pick_right_tan;

    tangents_mask( k, prev, next, pick_left_tan, pick_right_tan);

    if( pick_left_tan)
    {
        math::point2i_t tangent = view.left_tangent_pos( q, k.v0(), c.scale());
        if( view.inside_pick_distance( p, tangent))
        {
            left = true;
            return true;
        }
    }

    if( pick_right_tan)
    {
        math::point2i_t tangent = view.right_tangent_pos( q, k.v1(), c.scale());
        if( view.inside_pick_distance( p, tangent))
        {
            left = false;
            return true;
        }
    }

    return false;
}

int pick_tangent( const anim_curves_view_t& view,
                  const anim::float_curve_t& curve,
                  const math::point2i_t& p, bool& left)
{
    if( curve.size() >= 2)
    {
        if( pick_tangent( view, curve, curve[0], 0, &(curve[1]), p, left))
            return 0;

        for( int i = 1; i < curve.size() - 1; ++i)
        {
            if( pick_tangent( view, curve, curve[i], &(curve[i-1]), &(curve[i+1]), p, left))
                return i;
        }

        if( pick_tangent( view, curve, curve[ curve.size()-1], &(curve[ curve.size()-2]), 0, p, left))
            return curve.size()-1;
    }

    return -1;
}

} // unnamed

pick_tangents_visitor::pick_tangents_visitor( const anim_curves_view_t& view, const math::point2i_t& p) : view_( view)
{
    p_ = p;
    key_index = -1;
    left = false;
}

void pick_tangents_visitor::operator()( const anim::float_curve_t *c)
{
    RAMEN_ASSERT( key_index == -1);

    key_index = pick_tangent( view_, *c, p_, left);
}

pick_keyframe_visitor::pick_keyframe_visitor( const anim_curves_view_t& view, const math::point2i_t& p) : view_( view)
{
    p_ = p;
    key_index = -1;
}

void pick_keyframe_visitor::operator()( const anim::float_curve_t *c)
{
    RAMEN_ASSERT( key_index == -1);

    for( int i = 0; i < c->size(); ++i)
    {
        math::point2f_t q( (*c)[i].time(), c->relative_to_absolute( ( *c)[i].value()));
        math::point2i_t qi( view_.world_to_screen( q));

        if( view_.inside_pick_distance( p_, qi))
        {
            key_index = i;
            return;
        }
    }
}

void pick_keyframe_visitor::operator()( const anim::shape_curve2f_t *c)
{
    RAMEN_ASSERT( key_index == -1);

    for( int i = 0; i < c->size(); ++i)
    {
        math::point2f_t q( (*c)[i].time(), i);
        math::point2i_t qi( view_.world_to_screen( q));

        if( view_.inside_pick_distance( p_, qi))
        {
            key_index = i;
            return;
        }
    }
}

box_pick_keyframes_visitor::box_pick_keyframes_visitor( const anim_curves_view_t& view,
                                                        const math::box2i_t& box) : view_( view), box_( box) {}

void box_pick_keyframes_visitor::operator()( const anim::float_curve_t *c)
{
    for( int i = 0; i < c->size(); ++i)
    {
        math::point2f_t q( (*c)[i].time(), c->relative_to_absolute( (*c)[i].value()));
        math::point2i_t qi( view_.world_to_screen( q));

        if( box_.is_inside( qi))
        {
            (*c)[i].toggle_selection();
            any_picked = true;
        }
    }
}

void box_pick_keyframes_visitor::operator()( const anim::shape_curve2f_t *c)
{
    for( int i = 0; i < c->size(); ++i)
    {
        math::point2f_t q( (*c)[i].time(), i);
        math::point2i_t qi( view_.world_to_screen( q));

        if( box_.is_inside( qi))
        {
            (*c)[i].toggle_selection();
            any_picked = true;
        }
    }
}

} // ui
} // ramen
