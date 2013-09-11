// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_WORLD_VIEW_HPP
#define	RAMEN_UI_WORLD_VIEW_HPP

#include<ramen/ui/world_view/world_view_fwd.hpp>

#include<QWidget>
#include<QFont>

namespace ramen
{
namespace ui
{

class world_view_t : public QWidget
{
    Q_OBJECT

public:

    world_view_t( QWidget *parent = 0);
    ~world_view_t();

    QWidget *create_toolbar();

protected:

    virtual bool event( QEvent *event);
    virtual void keyPressEvent( QKeyEvent *event);
    virtual void keyReleaseEvent( QKeyEvent *event);
    virtual void mouseDoubleClickEvent( QMouseEvent *event);
    virtual void mouseMoveEvent( QMouseEvent *event);
    virtual void mousePressEvent( QMouseEvent *event);
    virtual void mouseReleaseEvent( QMouseEvent *event);
    virtual void paintEvent ( QPaintEvent *event);
    virtual void resizeEvent( QResizeEvent *event);
    virtual void contextMenuEvent( QContextMenuEvent *event);

private:

    QWidget *toolbar_;
    bool first_resize_;
    QFont font_;
};

} // ui
} // ramen

#endif
