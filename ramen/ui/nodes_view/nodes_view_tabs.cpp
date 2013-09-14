// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/nodes_view/nodes_view_tabs.hpp>

#include<ramen/nodes/world_node.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/ui/nodes_view/nodes_view.hpp>

namespace ramen
{
namespace ui
{

nodes_view_tabs_t::nodes_view_tabs_t( QWidget *parent) : QTabWidget( parent)
{
    setMovable( true);
    setTabsClosable( true);
    connect( this, SIGNAL( tabCloseRequested( int)), this, SLOT( delete_tab( int)));

    add_view( &( app().document().world_node()));
}

void nodes_view_tabs_t::add_view( nodes::composite_node_t *n)
{
    addTab( new nodes_view_t( n), n->name().c_str());
}

void nodes_view_tabs_t::update_state()
{
    update();
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
