// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_GRAPH_MODIFIER_HPP
#define RAMEN_NODES_GRAPH_MODIFIER_HPP

#include<ramen/nodes/node_graph_modifier_fwd.hpp>

#include<ramen/core/string8.hpp>
#include<ramen/core/memory.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{

class node_graph_modifier_t
{
public:

    node_graph_modifier_t( composite_node_t *graph, const core::string8_t& undo_name);
    ~node_graph_modifier_t();

    void add_node( BOOST_RV_REF( core::auto_ptr_t<node_t>) n)
    {
        // we need this workaround, because of a gcc? bug.
        do_add_node( n.release());
    }

    void remove_node( node_t *n);

    bool can_connect( node_t *src, node_t *dst, int port) const;

    void connect( node_t *src, node_t *dst, int port);
    void disconnect( node_t *src, node_t *dst, int port);

    void execute( bool undoable);

private:

    void do_add_node( node_t *n);

    // non-copyable
    node_graph_modifier_t( const node_graph_modifier_t&);
    node_graph_modifier_t& operator=( const node_graph_modifier_t&);

    composite_node_t *graph_;

    struct impl;
    impl *pimpl_;
};

} // ramen

#endif
