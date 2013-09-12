// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPGRAPH_DGRAPH_HPP
#define RAMEN_DEPGRAPH_DGRAPH_HPP

#include<ramen/depgraph/dgraph_fwd.hpp>

#include<ramen/depgraph/dnode_fwd.hpp>

namespace ramen
{
namespace depgraph
{

/**
\ingroup depgraph
\brief dependency graph
*/
class RAMEN_API dgraph_t
{
public:

    dgraph_t();

    void add_node( dnode_t *n);
    void remove_node( dnode_t *n);

private:

    // non-copyable
    dgraph_t( const dgraph_t&);
    dgraph_t& operator=( const dgraph_t&);
};

} // depgraph
} // ramen

#endif
