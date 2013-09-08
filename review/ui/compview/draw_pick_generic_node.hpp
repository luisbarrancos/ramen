// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COMPVIEW_DRAW_PICK_GENERIC_HPP
#define RAMEN_COMPVIEW_DRAW_PICK_GENERIC_HPP

#include<ramen/config.hpp>

#include<ramen/math/box2.hpp>

#include<ramen/ui/compview/pick_result.hpp>

class QPainter;

namespace ramen
{
namespace ui
{

class composition_view_t;

int generic_node_width( const node_t *n);
int generic_node_height();

math::point2f_t generic_input_location( const node_t *n, std::size_t i);
math::point2f_t generic_output_location( const node_t *n);

void draw_generic_node( QPainter& painter, const node_t *n);

void pick_generic_node( node_t *n,
                        const math::point2f_t& p,
                        const composition_view_t& view,
                        pick_result_t& result);

bool box_pick_generic_node( const node_t *n, const math::box2f_t& b);

} // ui
} // ramen

#endif
