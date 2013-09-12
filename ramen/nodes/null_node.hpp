// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_NULL_NODE_HPP
#define RAMEN_NODES_NULL_NODE_HPP

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace nodes
{

class null_node_t : public node_t
{
public:

    static const node_info_t& null_node_info();
    virtual const node_info_t *node_info() const;

    null_node_t();

protected:

    null_node_t( const null_node_t& other);

private:

    // non-assignable
    void operator=( const null_node_t&);

    node_t *do_clone() const;

    virtual bool do_is_identity() const;
};

} // nodes
} // ramen

#endif
