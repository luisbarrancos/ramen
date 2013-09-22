// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/viewer/viewer_tab_bar.hpp>

#include<QApplication>
#include<QMouseEvent>

#include<ramen/ui/viewer/viewer.hpp>
#include<ramen/ui/viewer/viewer_tabs.hpp>

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
    QTabBar::mouseMoveEvent( e);
}

void viewer_tabs_bar_t::mouseReleaseEvent( QMouseEvent *e)
{
    if( e->button() == Qt::LeftButton)
    {
        if( selected_tab_ != -1 && !rect().contains( e->pos()))
        {
            viewer_tabs_t *src_tabs = dynamic_cast<viewer_tabs_t*>( parent());

            if( QWidget *target = qApp->topLevelAt( e->globalPos()))
            {
                QPoint p = target->mapFromGlobal( e->globalPos());
                QWidget *w = target->childAt( p);
                while( w)
                {
                    if( w == src_tabs)
                        break;

                    if( viewer_tabs_t *dst_tabs = dynamic_cast<viewer_tabs_t*>( w))
                    {
                        src_tabs->transfer_viewer( selected_tab_, dst_tabs);
                        selected_tab_ = -1;
                        return;
                    }

                    w = w->parentWidget();
                }
            }

            src_tabs->detach_viewer( selected_tab_, e->globalPos());
            selected_tab_ = -1;
            return;
        }
    }

    QTabBar::mouseReleaseEvent( e);
}

} // ui
} // ramen
