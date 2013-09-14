// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_VIEWER_TABS_HPP
#define	RAMEN_UI_VIEWER_VIEWER_TABS_HPP

#include<ramen/ui/viewer/viewer_tabs_fwd.hpp>

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

class viewer_tabs_t : public QTabWidget
{
    Q_OBJECT

public:

    explicit viewer_tabs_t( QWidget *parent = 0);

    void add_viewer( const QString& name);
    void add_viewer( const QString& name, QWidget *tab);

    void update_state();

private:

    friend class viewer_tabs_bar_t;

    void detach_viewer( int index, const QPoint& pos);
    void transfer_viewer( int index, viewer_tabs_t *other);

private Q_SLOTS:

    void delete_tab( int index);
};

} // ui
} // ramen

#endif
