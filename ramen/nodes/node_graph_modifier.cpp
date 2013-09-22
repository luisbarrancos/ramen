// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/node_graph_modifier.hpp>

#include<ramen/assert.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/composite_node.hpp>

#include<ramen/undo/command.hpp>
#include<ramen/undo/stack.hpp>

namespace ramen
{
namespace undo
{

class create_node_command_t : public undo::command_t {};
class add_node_command_t : public undo::command_t {};
class remove_node_command_t : public undo::command_t {};
class connect_command_t : public undo::command_t {};
class disconnect_command_t : public undo::command_t {};
class ignore_node_command_t : public undo::command_t {};
class notify_all_command_t : public undo::command_t {};

} // undo

namespace nodes
{

struct node_graph_modifier_t::impl
{
    explicit impl( core::string8_t undo_name)
    {
    }

    void execute()
    {
        throw core::not_implemented();
    }
};

node_graph_modifier_t::node_graph_modifier_t( composite_node_t *graph,
                                              core::string8_t undo_name)
{
    RAMEN_ASSERT( graph);
    RAMEN_ASSERT( !undo_name.empty());

    graph_ = graph;
    pimpl_ = new impl( undo_name);
}

node_graph_modifier_t::~node_graph_modifier_t()
{
    delete pimpl_;
}

void node_graph_modifier_t::create_node( const core::name_t& id, bool ui)
{
    core::auto_ptr_t<node_t> n( graph_->create_node( id, ui));
    add_node( n);
}

void node_graph_modifier_t::create_node( const core::name_t& id,
                                         const std::pair<int, int>& version,
                                         bool ui)
{
    core::auto_ptr_t<node_t> n( graph_->create_node( id, version, ui));
    add_node( n);
}

void node_graph_modifier_t::do_add_node( node_t *n)
{
}

void node_graph_modifier_t::remove_node( node_t *n)
{
    RAMEN_ASSERT( n->parent() == graph_);
}

bool node_graph_modifier_t::can_connect( node_t *src, node_t *dst, int port) const
{
    return false;
}

void node_graph_modifier_t::connect( node_t *src, node_t *dst, int port)
{
    RAMEN_ASSERT( src);
    RAMEN_ASSERT( dst);
    RAMEN_ASSERT( src->parent() == graph_);
    RAMEN_ASSERT( dst->parent() == graph_);
}

void node_graph_modifier_t::disconnect( node_t *src, node_t *dst, int port)
{
    RAMEN_ASSERT( src);
    RAMEN_ASSERT( dst);
    RAMEN_ASSERT( src->parent() == graph_);
    RAMEN_ASSERT( dst->parent() == graph_);
}

void node_graph_modifier_t::ignore_node( node_t *n)
{
    RAMEN_ASSERT( n);
}

void node_graph_modifier_t::execute( bool undoable)
{
    pimpl_->execute();

    //if( undoable)
    //    app().document().undo_stack().push_back( boost::move( pimpl_->command));
}

} // nodes
} // ramen
