// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_GRAPH_MODIFIER_HPP
#define RAMEN_NODES_GRAPH_MODIFIER_HPP

#include<ramen/config.hpp>

#include<ramen/core/string8.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/undo/command_fwd.hpp>

namespace ramen
{

class node_graph_modifier_t
{
public:

    node_graph_modifier_t( composite_node_t *graph, const core::string8_t& undo_name);
    ~node_graph_modifier_t();

private:

};

} // ramen

#endif
