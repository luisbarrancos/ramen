// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composite_node.hpp>

#include<boost/range/algorithm/for_each.hpp>
#include<boost/bind.hpp>

#include<ramen/assert.hpp>

#include<ramen/container/ptr_vector_util.hpp>

#include<ramen/nodes/graph_algorithm.hpp>

namespace ramen
{

composite_node_t::composite_node_t() : node_t()
{
}

composite_node_t::composite_node_t( const composite_node_t& other) : node_t( other)
{
    throw core::not_implemented();
}

composite_node_t::~composite_node_t() {}

void composite_node_t::add_node( std::auto_ptr<node_t> n)
{
    RAMEN_ASSERT( n->parent() == 0);

    n->set_parent( this);
    nodes_.push_back( n);
}

std::auto_ptr<node_t> composite_node_t::release_node( node_t *n)
{
    RAMEN_ASSERT( n->parent() == this);

    std::auto_ptr<node_t> nn( container::release_ptr( n, nodes_));

    if( nn.get())
        nn->set_parent( 0);

    return nn;
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
    e.dst->connected( e.src, e.port);
}

void composite_node_t::remove_edge( const edge_t& e)
{
    RAMEN_ASSERT( e.src->parent() == this);
    RAMEN_ASSERT( e.dst->parent() == this);

    e.src->output_plug().remove_output( e.dst, e.port);
    e.dst->input_plugs()[e.port].clear_input();
    edges_.erase( std::find( edges_.begin(), edges_.end(), e));
    e.dst->connected( 0, e.port);
}

// connections
bool composite_node_t::can_connect( node_t *src, node_t *dst, int port)
{
    if( !src->has_output_plug() || dst->num_inputs() == 0)
        return false;

    if( port >= dst->num_inputs())
        return false;

    if( node_depends_on_node( *src, *dst))
        return false;

    return dst->accept_connection( src, port);
}

void composite_node_t::connect( node_t *src, node_t *dst, int port)
{
    add_edge( edge_t( src, dst, port));
}

void composite_node_t::disconnect( node_t *src, node_t *dst, int port)
{
    remove_edge( edge_t( src, dst, port));
}

void composite_node_t::do_set_frame( float f)
{
    boost::range::for_each( nodes(), boost::bind( &node_t::set_frame, _1, f));
}

} // ramen
