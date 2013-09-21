// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/depgraph/dgraph.hpp>

#include<ramen/assert.hpp>

#include<ramen/depgraph/exceptions.hpp>

#include<ramen/depgraph/dnode.hpp>

namespace ramen
{
namespace depgraph
{

dgraph_t::dgraph_t() {}

void dgraph_t::add_node( dnode_t *n)
{
    RAMEN_ASSERT( nodes_.count( n) == 0);

    nodes_.insert( n);
}

void dgraph_t::remove_node( dnode_t *n)
{
    RAMEN_ASSERT( nodes_.count( n) == 1);

    nodes_.erase( n);
}

void dgraph_t::add_dependency( const dependency_t& d)
{
    RAMEN_ASSERT( nodes_.count( d.src) == 1);
    RAMEN_ASSERT( nodes_.count( d.dst) == 1);
    RAMEN_ASSERT( dependencies_.count( d) == 0);

    dependencies_.insert( d);
    throw core::not_implemented();
}

void dgraph_t::remove_dependency( const dependency_t& d)
{
    RAMEN_ASSERT( dependencies_.count( d) == 1);

    dependencies_.erase( d);
    throw core::not_implemented();
}

void dgraph_t::propagate_dirty_flags()
{
    while( true)
    {
        bool done = true;
        for( std::set<dependency_t>::iterator it( dependencies_.begin()), e( dependencies_.end());
             it != e; ++it)
        {
            if( it->src->dirty())
            {
                it->dst->set_dirty( true);
                done = false;
            }
        }

        if( done)
            break;
    }
}

} // depgraph
} // ramen
