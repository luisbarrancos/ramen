// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/nodes_view/nodes_view.hpp>

#include<QKeyEvent>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QResizeEvent>
#include<QHelpEvent>
#include<QPainter>
#include<QPen>
#include<QBrush>
#include<QToolTip>
#include<QAction>
#include<QContextMenuEvent>
#include<QMenu>

#include<ramen/assert.hpp>

namespace ramen
{
namespace ui
{

nodes_view_t::nodes_view_t( nodes::composite_node_t *n) : QWidget()
{
    init( n);
}

nodes_view_t::nodes_view_t( QWidget *parent, nodes::composite_node_t *n) : QWidget( parent)
{
    init( n);
}

void nodes_view_t::init( nodes::composite_node_t *n)
{
    RAMEN_ASSERT( n);

    setFocusPolicy( Qt::ClickFocus);
    font_ = QFont( "Helvetica", 10);
    first_resize_ = true;
    node_ = n;
}

bool nodes_view_t::event( QEvent *event)
{
    return QWidget::event( event);
}

void nodes_view_t::keyPressEvent( QKeyEvent *event)
{
    event->ignore();
}

void nodes_view_t::keyReleaseEvent( QKeyEvent *event)
{
    event->ignore();
}

void nodes_view_t::mouseDoubleClickEvent( QMouseEvent *event)
{
    event->ignore();
}

void nodes_view_t::mousePressEvent( QMouseEvent *event)
{
    event->ignore();
}

void nodes_view_t::mouseMoveEvent( QMouseEvent *event)
{
    event->ignore();
}

void nodes_view_t::mouseReleaseEvent( QMouseEvent *event)
{
    event->ignore();
}

void nodes_view_t::resizeEvent( QResizeEvent *event)
{
    if( first_resize_)
    {
        if( event->size().width() == 0 || event->size().height() == 0)
        {
            event->accept();
            return;
        }

        first_resize_ = false;
        //viewport().reset( event->size().width(), event->size().height());
    }
    else
        ; //viewport().resize( event->size().width(), event->size().height());

    event->accept();
}

void nodes_view_t::paintEvent ( QPaintEvent *event)
{
    event->accept();
}

void nodes_view_t::contextMenuEvent( QContextMenuEvent *event)
{
    event->accept();
}

} // ui
} // ramen
