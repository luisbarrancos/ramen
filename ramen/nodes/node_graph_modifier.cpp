// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/node_graph_modifier.hpp>

#include<ramen/assert.hpp>

namespace ramen
{

node_graph_modifier_t::node_graph_modifier_t( composite_node_t *graph,
                                              const core::string8_t& undo_name)
{
    RAMEN_ASSERT( graph);
    RAMEN_ASSERT( !undo_name.empty());
}

node_graph_modifier_t::~node_graph_modifier_t()
{
}

} // ramen
