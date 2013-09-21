// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPGRAPH_DEPENDENCY_HPP
#define RAMEN_DEPGRAPH_DEPENDENCY_HPP

#include<ramen/depgraph/dnode_fwd.hpp>

#include<boost/operators.hpp>

namespace ramen
{
namespace depgraph
{

/**
\ingroup depgraph
\brief represents a dependency between two nodes.
*/
class RAMEN_API dependency_t : boost::equality_comparable<dependency_t>
{
public:

    dependency_t( dnode_t *s, dnode_t *d);

    bool operator==( const dependency_t& other) const;

    bool operator<( const dependency_t& other) const;

    dnode_t *src, *dst;
};

} // depgraph
} // ramen

#endif
