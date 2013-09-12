// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_GRAPH_MODIFIER_FWD_HPP
#define RAMEN_NODES_GRAPH_MODIFIER_FWD_HPP

#include<ramen/config.hpp>

namespace ramen
{
namespace undo
{

class add_node_command_t;
class remove_node_command_t;
class connect_command_t;
class disconnect_command_t;
class ignore_node_command_t;

} // undo

namespace nodes
{

class node_graph_modifier_t;

} // nodes
} // ramen

#endif
