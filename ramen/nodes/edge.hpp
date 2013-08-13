// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_EDGE_HPP
#define RAMEN_NODES_EDGE_HPP

#include<ramen/config.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{

struct edge_t
{
    edge_t();
    edge_t( node_t *s=0, node_t *d=0, int p=-1);

    bool operator==( const edge_t& other) const;
    bool operator!=( const edge_t& other) const
    {
        return !( *this == other);
    }

    bool operator<( const edge_t& other) const;

    node_t *src;
    node_t *dst;
    int port;
};

} // ramen

#endif
