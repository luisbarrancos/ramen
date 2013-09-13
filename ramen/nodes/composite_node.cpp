// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composite_node.hpp>

#include<boost/range/algorithm/for_each.hpp>
#include<boost/bind.hpp>
#include<boost/foreach.hpp>

#include<ramen/assert.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/nodes/node_factory.hpp>

namespace ramen
{
namespace nodes
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
    RAMEN_ASSERT( n.get());

    node_t *nn = n.get();
    n->set_parent( this);
    nodes_.push_back( n.release());

    composite_node_t *g = this;
    while( g)
    {
        g->emit_node_added_signal( nn);
        g = g->parent();
    }
}

void composite_node_t::emit_node_added_signal( node_t *n)
{
    node_added( n);

    if( composite_node_t *c = dynamic_cast<composite_node_t*>( n))
    {
        BOOST_FOREACH( node_t& child, c->nodes())
        {
            emit_node_added_signal( &child);
        }
    }
}

core::auto_ptr_t<node_t> composite_node_t::release_node( node_t *n)
{
    RAMEN_ASSERT( n);
    RAMEN_ASSERT( n->parent() == this);

    core::auto_ptr_t<node_t> nn( nodes_.release_ptr( n));

    if( nn.get())
        nn->set_parent( 0);

    composite_node_t *g = this;
    while( g)
    {
        g->emit_node_released_signal( n);
        g = g->parent();
    }

    return core::auto_ptr_t<node_t>( nn.release());
}

void composite_node_t::emit_node_released_signal( node_t *n)
{
    if( composite_node_t *c = dynamic_cast<composite_node_t*>( n))
    {
        BOOST_FOREACH( node_t& child, c->nodes())
        {
            emit_node_released_signal( &child);
        }
    }

    node_released( n);
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

} // nodes
} // ramen
