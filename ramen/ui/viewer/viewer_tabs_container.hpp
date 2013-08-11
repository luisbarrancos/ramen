// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_VIEWER_TABS_CONTAINER_HPP
#define	RAMEN_UI_VIEWER_VIEWER_TABS_CONTAINER_HPP

#include<ramen/config.hpp>

#include<QTabWidget>
#include<QTabBar>

class QEvent;
class QMouseEvent;
class QDragEvent;
class QDropEvent;

namespace ramen
{
namespace ui
{

class viewer_tabs_bar_t : public QTabBar
{
    Q_OBJECT

public:

    explicit viewer_tabs_bar_t( QWidget *parent = 0);

protected:

    virtual void mousePressEvent( QMouseEvent *e);
    virtual void mouseMoveEvent( QMouseEvent *e);
    virtual void mouseReleaseEvent( QMouseEvent *e);

private:

    int selected_tab_;
};

class viewer_tabs_container_t : public QTabWidget
{
    Q_OBJECT

public:

    explicit viewer_tabs_container_t( QWidget *parent = 0);

    void add_tab( const QString& name);
    void add_tab( const QString& name, QWidget *tab);

    void detach_tab( int index, const QPoint& pos);
    void transfer_tab( int index, viewer_tabs_container_t *other_tabs);

private Q_SLOTS:

    void delete_tab( int index);
};

} // ui
} // ramen

#endif
