// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/pointop_node.hpp>

namespace ramen
{

pointop_node_t::pointop_node_t() : node_t()
{
    add_input_plug( "Front", ui::palette_t::instance().color("front plug"), false);
    add_output_plug();
}

pointop_node_t::pointop_node_t( const pointop_node_t& other) : node_t( other) {}

void pointop_node_t::do_process( const render::context_t& context)
{
    math::box2i_t area( math::intersect( input()->defined(), defined()));

    if( area.is_empty())
        return;

    do_process( input()->const_subimage_view( area), subimage_view( area), context);
}

} // ramen
