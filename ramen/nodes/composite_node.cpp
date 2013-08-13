// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composite_node.hpp>

#include<boost/range/algorithm/for_each.hpp>
#include<boost/bind.hpp>

#include<ramen/assert.hpp>

#include<ramen/nodes/node_factory.hpp>
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

void composite_node_t::add_node( BOOST_RV_REF( core::auto_ptr_t<node_t>) n)
{
    n->set_parent( this);
    nodes_.push_back( n.release());
}

core::auto_ptr_t<node_t> composite_node_t::release_node( node_t *n)
{
    RAMEN_ASSERT( n->parent() == this);

    core::auto_ptr_t<node_t> nn( nodes_.release_ptr( n));

    if( nn.get())
        nn->set_parent( 0);

    return core::auto_ptr_t<node_t>( nn.release());
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
    edge_iterator it( std::find( edges_.begin(), edges_.end(), e));
    RAMEN_ASSERT( it != edges_.end());

    e.src->output_plug().remove_output( e.dst, e.port);
    e.dst->input_plugs()[e.port].clear_input();
    edges_.erase( it);
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
    RAMEN_ASSERT( src->parent() == this);
    RAMEN_ASSERT( dst->parent() == this);

    add_edge( edge_t( src, dst, port));
}

void composite_node_t::disconnect( node_t *src, node_t *dst, int port)
{
    RAMEN_ASSERT( src->parent() == this);
    RAMEN_ASSERT( dst->parent() == this);

    remove_edge( edge_t( src, dst, port));
}

void composite_node_t::do_set_frame( float f)
{
    boost::range::for_each( nodes(), boost::bind( &node_t::set_frame, _1, f));
}

core::auto_ptr_t<node_t> composite_node_t::create_node( const core::name_t& id, bool ui)
{
    core::auto_ptr_t<node_t> p( node_factory_t::instance().create_by_id( id, ui));

    RAMEN_ASSERT( p.get());

    try
    {
        p->set_parent( this);
        p->create_params();
        p->create_manipulators();
    }
    catch( ...)
    {
        return core::auto_ptr_t<node_t>();
    }

    return core::auto_ptr_t<node_t>( p.release());
}

core::auto_ptr_t<node_t> composite_node_t::create_node( const core::name_t& id,
                                                        const std::pair<int,int>& version)
{
    RAMEN_ASSERT( !id.empty());
    RAMEN_ASSERT( version.first >= 0 && version.second >= 0);

    core::auto_ptr_t<node_t> p( node_factory_t::instance().create_by_id_with_version( id, version));

    // as a last resort, return an unknown node
    if( !p.get())
        return create_unknown_node( id, version);

    try
    {
        p->set_parent( this);
        p->create_params();
        p->create_manipulators();
    }
    catch( ...)
    {
        return create_unknown_node( id, version);
    }

    return core::auto_ptr_t<node_t>( p.release());
}

core::auto_ptr_t<node_t> composite_node_t::create_unknown_node( const core::name_t& id,
                                                                const std::pair<int, int>& version)
{
    // TODO: implement this.
    throw core::not_implemented();

    return core::auto_ptr_t<node_t>();
}

void composite_node_t::do_begin_interaction()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::begin_interaction, _1));
}

void composite_node_t::do_end_interaction()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::end_interaction, _1));
}

} // ramen
