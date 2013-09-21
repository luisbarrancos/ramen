// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/depgraph/dependency.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace depgraph
{

dependency_t::dependency_t( dnode_t *s, dnode_t *d)
{
    RAMEN_ASSERT( s);
    RAMEN_ASSERT( d);
    RAMEN_ASSERT( s != d);

    src = s;
    dst = d;
}

bool dependency_t::operator==( const dependency_t& other) const
{
    return src == other.src && dst == other.dst;
}

bool dependency_t::operator<( const dependency_t& other) const
{
    if( src < other.src)
        return true;

    if( src > other.src)
        return false;

    return dst < other.dst;
}

} // depgraph
} // ramen
