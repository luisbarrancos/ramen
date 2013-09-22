// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/nodes_view/nodes_view_tabs.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/world_node.hpp>

#include<ramen/ui/nodes_view/nodes_view.hpp>

namespace ramen
{
namespace ui
{

nodes_view_tabs_t::nodes_view_tabs_t( QWidget *parent) : QTabWidget( parent), active_view_( -1)
{
    setMovable( true);
    setTabsClosable( true);
    connect( this, SIGNAL( tabCloseRequested( int)), this, SLOT( delete_tab( int)));

    add_view( &( app().document().world_node()));

    nodes::world_node_t::node_deleted.connect( boost::bind( &nodes_view_tabs_t::node_was_deleted, this, _1));
    nodes::world_node_t::node_renamed.connect( boost::bind( &nodes_view_tabs_t::node_was_renamed, this, _1, _2, _3));
}

void nodes_view_tabs_t::add_view( nodes::composite_node_t *n)
{
    nodes_view_t *view = new nodes_view_t( n);
    views_.push_back( view);
    active_view_ = views_.size() - 1;
    addTab( view, n->name().c_str());
}

void nodes_view_tabs_t::update_state()
{
    update();
}

const nodes_view_t *nodes_view_tabs_t::active_view() const
{
    if( active_view_ >= 0)
        return views_[active_view_];

    return 0;
}

nodes_view_t *nodes_view_tabs_t::active_view()
{
    if( active_view_ >= 0)
        return views_[active_view_];

    return 0;
}

void nodes_view_tabs_t::node_was_deleted( nodes::node_t *n)
{
    if( nodes::composite_node_t *c = dynamic_cast<nodes::composite_node_t*>( n))
    {
        if( c != &( app().document().world_node()))
            return;

        // TODO: check if we have a tab for this node and close it.
    }
}

void nodes_view_tabs_t::node_was_renamed( nodes::node_t* n,
                                          const core::string8_t& old_name,
                                          const core::string8_t& new_name)
{
    if( nodes::composite_node_t *c = dynamic_cast<nodes::composite_node_t*>( n))
    {
        // TODO: check if we have a tab for this node and rename it.
    }
}

void nodes_view_tabs_t::delete_tab( int index)
{
    nodes_view_t *view = dynamic_cast<nodes_view_t*>( widget( index));

    // don't remove the world tab.
    if( view->node() != &( app().document().world_node()))
    {
        removeTab( index);
        delete view;
    }
}

} // ui
} // ramen
