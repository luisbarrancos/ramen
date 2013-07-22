// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composite_node.hpp>

#include<ramen/assert.hpp>

#include<ramen/container/ptr_vector_util.hpp>

namespace ramen
{

composite_node_t::composite_node_t() : node_t()
{
}

composite_node_t::composite_node_t( const composite_node_t& other) : node_t( other)
{
    throw core::not_implemented();
}

void composite_node_t::add_node( std::auto_ptr<node_t> n)
{
    n->set_parent( this);
    nodes_.push_back( n);
}

std::auto_ptr<node_t> composite_node_t::release_node( node_t *n)
{
    RAMEN_ASSERT( n->parent() == this);

    n->set_parent( 0);
    return container::release_ptr( n, nodes_);
}

void composite_node_t::add_edge( const edge_t& e)
{
    RAMEN_ASSERT( e.src->parent() == this);
    RAMEN_ASSERT( e.dst->parent() == this);

    node_t *src = e.dst->input_plugs()[e.port].input_node();

    if( src)
        remove_edge( edge_t( src, e.dst, e.port));

    e.dst->input_plugs()[e.port].set_input( e.src);
    e.src->output_plug().add_output( e.dst, e.port);
    edges_.push_back( e);
}

void composite_node_t::remove_edge( const edge_t& e)
{
    RAMEN_ASSERT( e.src->parent() == this);
    RAMEN_ASSERT( e.dst->parent() == this);

    e.src->output_plug().remove_output( e.dst, e.port);
    e.dst->input_plugs()[e.port].clear_input();
    edges_.erase( std::find( edges_.begin(), edges_.end(), e));
}

// connections
void composite_node_t::connect( node_t *src, node_t *dst, int port)
{
    add_edge( edge_t( src, dst, port));
}

void composite_node_t::disconnect( node_t *src, node_t *dst, int port)
{
    remove_edge( edge_t( src, dst, port));
}

} // ramen
