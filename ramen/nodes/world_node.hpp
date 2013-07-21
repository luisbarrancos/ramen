// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_WORLD_NODE_HPP
#define RAMEN_NODES_WORLD_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/nodes/composite_node.hpp>

namespace ramen
{

class world_node_t : public composite_node_t
{
public:

private:

    void operator=( const world_node_t& other);
};

} // ramen

#endif
