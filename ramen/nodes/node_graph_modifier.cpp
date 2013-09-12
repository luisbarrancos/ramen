// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/node_graph_modifier.hpp>

#include<set>

#include<ramen/assert.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/composite_node.hpp>

#include<ramen/undo/command.hpp>
#include<ramen/undo/stack.hpp>

namespace ramen
{
namespace undo
{

class add_node_command_t : public undo::command_t
{
public:

    add_node_command_t( nodes::composite_node_t *graph,
                        nodes::node_t *n) : undo::command_t(), graph_( graph)
    {
        RAMEN_ASSERT( graph);
        RAMEN_ASSERT( n);

        node_ = n;
        storage_.reset( n);
    }

    virtual void undo()
    {
        storage_ = graph_->release_node( node_);
    }

    virtual void redo()
    {
        graph_->add_node( boost::move( storage_));
    }

private:

    nodes::composite_node_t *graph_;
    nodes::node_t *node_;
    core::auto_ptr_t<nodes::node_t> storage_;
};

class remove_node_command_t : public undo::command_t
{
public:

    remove_node_command_t( nodes::composite_node_t *graph,
                           nodes::node_t *n) : undo::command_t(),
                                               graph_( graph),
                                               node_( n)
    {
        RAMEN_ASSERT( graph);
        RAMEN_ASSERT( n);
    }

    virtual void undo()
    {
        graph_->add_node( storage_);
    }

    virtual void redo()
    {
        storage_ = graph_->release_node( node_);
    }

private:

    nodes::composite_node_t *graph_;
    nodes::node_t *node_;
    core::auto_ptr_t<nodes::node_t> storage_;
};

class connect_command_t : public undo::command_t
{
public:

    connect_command_t( nodes::composite_node_t *graph,
                       nodes::node_t *src,
                       nodes::node_t *dst,
                       int port) : undo::command_t()
    {
        RAMEN_ASSERT( graph);
        RAMEN_ASSERT( src);
        RAMEN_ASSERT( dst);
        RAMEN_ASSERT( port >= 0);

        graph_ = graph;
        src_ = src;
        dst_ = dst;
        port_ = port;
    }

    virtual void undo()
    {
        //graph_->disconnect( src_, dst_, port_);
    }

    virtual void redo()
    {
        //graph_->connect( src_, dst_, port_);
    }

private:

    nodes::composite_node_t *graph_;
    nodes::node_t *src_;
    nodes::node_t *dst_;
    int port_;
};

class disconnect_command_t : public undo::command_t
{
public:

    disconnect_command_t( nodes::composite_node_t *graph,
                          nodes::node_t *src,
                          nodes::node_t *dst,
                          int port) : undo::command_t()
    {
        RAMEN_ASSERT( graph);
        RAMEN_ASSERT( src);
        RAMEN_ASSERT( dst);
        RAMEN_ASSERT( port >= 0);

        graph_ = graph;
        src_ = src;
        dst_ = dst;
        port_ = port;
    }

    virtual void undo()
    {
        //graph_->connect( src_, dst_, port_);
    }

    virtual void redo()
    {
        //graph_->disconnect( src_, dst_, port_);
    }

private:

    nodes::composite_node_t *graph_;
    nodes::node_t *src_;
    nodes::node_t *dst_;
    int port_;
};

class ignore_node_command_t : public undo::command_t
{
public:

    ignore_node_command_t( nodes::composite_node_t *graph,
                           nodes::node_t *n) : undo::command_t()
    {
        RAMEN_ASSERT( graph);
        RAMEN_ASSERT( n);

        graph_ = graph;
        n_ = n;
    }

    virtual void undo()
    {
        //n_->set_ignored( !n_->ignored());
    }

    virtual void redo()
    {
        //n_->set_ignored( !n_->ignored());
    }

private:

    nodes::composite_node_t *graph_;
    nodes::node_t *n_;
};

class notify_all_command_t : public undo::command_t
{
public:

    notify_all_command_t() : undo::command_t()
    {
    }

    virtual void undo()
    {
    }

    virtual void redo()
    {
    }

    //std::set<node_t*> nodes_to_add;
    //std::set<node_t*> nodes_to_remove;
    //std::set<edge_t> edges_to_add;
    //std::set<edge_t> edges_to_remove;
};

} // undo

namespace nodes
{

struct node_graph_modifier_t::impl
{
    explicit impl( core::string8_t undo_name)
    {
        command.reset( new undo::composite_command_t( undo_name));
        notify_command.reset( new undo::notify_all_command_t());
    }

    void execute()
    {
        core::auto_ptr_t<undo::command_t> tmp( notify_command.release());
        command->push_back( boost::move( tmp));
        command->redo();
    }

    core::auto_ptr_t<undo::composite_command_t> command;
    core::auto_ptr_t<undo::notify_all_command_t> notify_command;
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

void node_graph_modifier_t::do_add_node( node_t *n)
{
    //pimpl_->notify_command->nodes_to_add.insert( n);
    //core::auto_ptr_t<undo::command_t> c( new undo::add_node_command_t( graph_, n));
    //pimpl_->command->push_back( boost::move( c));
}

void node_graph_modifier_t::remove_node( node_t *n)
{
    RAMEN_ASSERT( n->parent() == graph_);
    /*
    for( int i = 0; i < n->num_inputs(); ++i)
    {
        if( n->input( i))
            disconnect( n->input( i), n, i);
    }

    if( n->num_outputs())
    {
        for( int i = 0, e = n->output_plug().connections().size(); i < e; ++i)
        {
            node_t *dst = boost::get<0>( n->output_plug().connections()[i]);
            int port    = boost::get<1>( n->output_plug().connections()[i]);
            disconnect( n, dst, port);
        }
    }

    pimpl_->notify_command->nodes_to_remove.insert( n);
    core::auto_ptr_t<undo::command_t> c( new undo::remove_node_command_t( graph_, n));
    pimpl_->command->push_back( boost::move( c));
    */
}

bool node_graph_modifier_t::can_connect( node_t *src, node_t *dst, int port) const
{
    //return graph_->can_connect( src, dst, port);
    return false;
}

void node_graph_modifier_t::connect( node_t *src, node_t *dst, int port)
{
    /*
    RAMEN_ASSERT( src);
    RAMEN_ASSERT( dst);
    RAMEN_ASSERT( port >= 0);
    RAMEN_ASSERT( src->parent() == graph_);
    RAMEN_ASSERT( dst->parent() == graph_);

    if( node_t *prev_src = dst->input_plugs()[port].input_node())
    {
        edge_t e( prev_src, dst, port);
        if( !pimpl_->notify_command->edges_to_remove.count( e))
            disconnect( prev_src, dst, port);
    }

    core::auto_ptr_t<undo::command_t> c( new undo::connect_command_t( graph_, src, dst, port));
    pimpl_->command->push_back( boost::move( c));
    */
}

void node_graph_modifier_t::disconnect( node_t *src, node_t *dst, int port)
{
    /*
    RAMEN_ASSERT( src);
    RAMEN_ASSERT( dst);
    RAMEN_ASSERT( port >= 0);
    RAMEN_ASSERT( src->parent() == graph_);
    RAMEN_ASSERT( dst->parent() == graph_);

    core::auto_ptr_t<undo::command_t> c( new undo::disconnect_command_t( graph_, src, dst, port));
    pimpl_->command->push_back( boost::move( c));
    pimpl_->notify_command->edges_to_add.insert( edge_t( src, dst, port));
    */
}

void node_graph_modifier_t::ignore_node( node_t *n)
{
    /*
    RAMEN_ASSERT( n);

    core::auto_ptr_t<undo::command_t> c( new undo::ignore_node_command_t( graph_, n));
    pimpl_->command->push_back( boost::move( c));
    */
}

void node_graph_modifier_t::execute( bool undoable)
{
    pimpl_->execute();

    if( undoable)
        app().document().undo_stack().push_back( boost::move( pimpl_->command));
}

} // nodes
} // ramen
