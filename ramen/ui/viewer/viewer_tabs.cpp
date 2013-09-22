// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/viewer/viewer_tabs.hpp>

#include<QMouseEvent>

#include<ramen/nodes/world_node.hpp>
#include<ramen/nodes/viewer_node.hpp>

#include<ramen/ui/viewer/viewer.hpp>

namespace ramen
{
namespace ui
{

viewer_tabs_t::viewer_tabs_t( QWidget *parent) : QTabWidget( parent)
{
    setTabBar( new viewer_tabs_bar_t());

    setMovable( true);
    setTabsClosable( true);
    connect( this, SIGNAL( tabCloseRequested( int)), this, SLOT( delete_tab( int)));

    nodes::world_node_t::node_added.connect(  boost::bind(  &viewer_tabs_t::node_was_added  , this, _1));
    nodes::world_node_t::node_deleted.connect( boost::bind( &viewer_tabs_t::node_was_deleted, this, _1));
    nodes::world_node_t::node_renamed.connect( boost::bind( &viewer_tabs_t::node_was_renamed, this, _1, _2, _3));
}

void viewer_tabs_t::update_state()
{
    update();
}

void viewer_tabs_t::add_viewer( const QString& name)
{
    addTab( new viewer_t( 0), name);
}

void viewer_tabs_t::detach_viewer( int index, const QPoint& pos)
{
    QWidget *tab = widget( index);
    int w = tab->width();
    int h = tab->height();

    QString name = tabText( index);
    removeTab( index);

    viewer_tabs_t *window = new viewer_tabs_t();
    window->setWindowFlags( Qt::Window);
    window->setWindowTitle( name);
    window->addTab( tab, name);
    window->move( pos);
    window->resize( w, h);
    window->show();

    if( !parent() && !count())
        close();
}

void viewer_tabs_t::transfer_viewer( int index, viewer_tabs_t *other)
{
    QWidget *tab = widget( index);
    QString name = tabText( index);
    removeTab( index);

    other->addTab( tab, name);

    if( !parent() && !count())
        close();
}

void viewer_tabs_t::node_was_added( nodes::node_t *n)
{
    if( dynamic_cast<nodes::viewer_node_t*>( n))
        add_viewer( n->name().c_str());
}

void viewer_tabs_t::node_was_deleted( nodes::node_t *n)
{
    if( nodes::viewer_node_t *v = dynamic_cast<nodes::viewer_node_t*>( n))
    {
        // TODO: remove viewer here...
    }
}

void viewer_tabs_t::node_was_renamed( nodes::node_t* n,
                                      const core::string8_t& old_name,
                                      const core::string8_t& new_name)
{
    if( nodes::viewer_node_t *v = dynamic_cast<nodes::viewer_node_t*>( n))
    {
        // TODO: rename viewer here...
    }
}

void viewer_tabs_t::delete_tab( int index)
{
    QWidget *tab = widget( index);
    removeTab( index);
    delete tab;

    if( !parent())
        close();
}

} // ui
} // ramen
