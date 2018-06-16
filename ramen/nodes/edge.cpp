// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/nodes/edge.hpp>

#include <cassert>

namespace ramen
{
edge_t::edge_t()
: src(0)
, dst(0)
, port(-1)
{
}

edge_t::edge_t(node_t* s, node_t* d, int p)
: src(s)
, dst(d)
, port(p)
{
    assert(s);
    assert(d);
}

edge_t::edge_t(node_t* s, const core::name_t& splug, node_t* d, const core::name_t& dplug)
: src(s)
, src_plug(splug)
, dst(d)
, dst_plug(dplug)
{
    assert(s);
    assert(d);
}

bool edge_t::operator==(const edge_t& other) const
{
    return src == other.src && dst == other.dst && port == other.port;
}

}  // ramen
