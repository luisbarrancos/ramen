// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composition_node.hpp>

#include<ramen/core/exceptions.hpp>

namespace ramen
{

composition_node_t::composition_node_t() : composite_node_t()
{
}

composition_node_t::composition_node_t( const composition_node_t& other) : composite_node_t( other)
{
    throw core::not_implemented();
}

node_t *composition_node_t::do_clone() const
{
    return new composition_node_t( *this);
}

void composition_node_t::do_create_params()
{

}

} // ramen
