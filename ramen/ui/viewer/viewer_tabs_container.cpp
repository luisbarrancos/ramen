// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/viewer/viewer_tabs_container.hpp>

#include<QMouseEvent>

#include<iostream>

namespace ramen
{
namespace ui
{

viewer_tabs_bar_t::viewer_tabs_bar_t( QWidget *parent) : QTabBar( parent)
{
    selected_tab_ = -1;
}

void viewer_tabs_bar_t::mousePressEvent( QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton)
        selected_tab_ = tabAt( e->pos());

    QTabBar::mousePressEvent( e);
}

void viewer_tabs_bar_t::mouseMoveEvent( QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton)
    {
        if( selected_tab_ != -1 && !rect().contains( e->pos()))
        {
        }
    }

    QTabBar::mouseMoveEvent( e);
}

void viewer_tabs_bar_t::mouseReleaseEvent( QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton)
    {
        if( selected_tab_ != -1 && !rect().contains( e->pos()))
        {
            if( viewer_tabs_container_t *c = dynamic_cast<viewer_tabs_container_t*>( parent()))
                c->detach_tab( selected_tab_, e->globalPos());
        }

        selected_tab_ = -1;
    }

    QTabBar::mouseReleaseEvent( e);
}

void viewer_tabs_bar_t::leaveEvent( QEvent* e)
{
    QTabBar::leaveEvent( e);
}

/*************************************************************/

viewer_detached_tab_t::viewer_detached_tab_t()
{
    setMovable( true);
    setTabsClosable( true);
    connect( this, SIGNAL( tabCloseRequested( int)), this, SLOT( delete_tab( int)));
}

void viewer_detached_tab_t::delete_tab( int index)
{
    close();
}

/*************************************************************/

viewer_tabs_container_t::viewer_tabs_container_t( QWidget *parent) : QTabWidget( parent)
{
    setTabBar( new viewer_tabs_bar_t());

    setMovable( true);
    setTabsClosable( true);
    connect( this, SIGNAL( tabCloseRequested( int)), this, SLOT( delete_tab( int)));

    add_tab( "Viewer 1");
    add_tab( "Viewer 2");
    add_tab( "Viewer 3");
}

void viewer_tabs_container_t::add_tab( const QString& name)
{
    addTab( new QWidget(), name);
}

void viewer_tabs_container_t::detach_tab( int index, const QPoint& pos)
{
    QWidget *tab = widget( index);
    QString name = tabText( index);
    removeTab( index);

    viewer_detached_tab_t *window = new viewer_detached_tab_t();
    window->setWindowFlags( Qt::Window);
    window->setWindowTitle( name);
    window->addTab( tab, name);
    window->move( pos);
    window->show();
}

void viewer_tabs_container_t::delete_tab( int index)
{
    QWidget *tab = widget( index);
    removeTab( index);
    delete tab;
}

} // ui
} // ramen
