// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/depgraph/dnode.hpp>

#include<ramen/depgraph/dgraph.hpp>

#include<ramen/app/application.hpp>

namespace ramen
{
namespace depgraph
{

dnode_t::dnode_t()
{
    app().dependency_graph().add_node( this);
}

dnode_t::dnode_t( const dnode_t& other)
{
    app().dependency_graph().add_node( this);
}

dnode_t::~dnode_t()
{
    app().dependency_graph().remove_node( this);
}

} // depgraph
} // ramen
