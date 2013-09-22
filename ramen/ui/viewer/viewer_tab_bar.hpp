// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_VIEWER_TAB_BAR_HPP
#define	RAMEN_UI_VIEWER_VIEWER_TAB_BAR_HPP

#include<ramen/config.hpp>

#include<QTabBar>

class QMouseEvent;

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

} // ui
} // ramen

#endif
