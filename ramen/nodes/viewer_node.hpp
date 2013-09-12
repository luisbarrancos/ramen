// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_VIEWER_NODE_HPP
#define RAMEN_NODES_VIEWER_NODE_HPP

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace nodes
{

class RAMEN_API viewer_node_t : public node_t
{
public:

    static const node_info_t& viewer_node_info();
    virtual const node_info_t *node_info() const;

    viewer_node_t();

protected:

    viewer_node_t( const viewer_node_t& other);

private:

    void operator=( const viewer_node_t&);

    node_t *do_clone() const { return new viewer_node_t(*this);}
};

} // nodes
} // ramen

#endif
