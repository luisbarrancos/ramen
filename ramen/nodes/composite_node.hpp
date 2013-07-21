// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITE_NODE_HPP
#define RAMEN_NODES_COMPOSITE_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/nodes/node.hpp>

namespace ramen
{

/**
\ingroup nodes
\brief A collection of nodes and connections between them.
*/
class composite_node_t : public node_t
{
public:

private:

    void operator=( const composite_node_t& other);
};

} // ramen

#endif
