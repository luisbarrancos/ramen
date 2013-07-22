// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITION_NODE_HPP
#define RAMEN_NODES_COMPOSITION_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/nodes/composite_node.hpp>

namespace ramen
{

class composition_node_t : public composite_node_t
{
public:

    /// Constructor
    composition_node_t();

protected:

    composition_node_t( const composition_node_t& other);

private:

    void operator=( const composition_node_t& other);

    virtual node_t *do_clone() const;

    virtual void do_create_params();
};

} // ramen

#endif
