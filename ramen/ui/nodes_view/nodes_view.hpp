// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_NODES_VIEW_NODES_VIEW_HPP
#define	RAMEN_UI_NODES_VIEW_NODES_VIEW_HPP

#include<ramen/ui/nodes_view/nodes_view_fwd.hpp>

#include<QWidget>
#include<QFont>

#include<ramen/nodes/composite_node.hpp>

namespace ramen
{
namespace ui
{

class nodes_view_t : public QWidget
{
    Q_OBJECT

public:

    explicit nodes_view_t( nodes::composite_node_t *n);
    nodes_view_t( QWidget *parent, nodes::composite_node_t *n);

    const nodes::composite_node_t *node() const { return node_;}
    nodes::composite_node_t *node()             { return node_;}

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

    void init( nodes::composite_node_t *n);

    nodes::composite_node_t *node_;

    bool first_resize_;
    QFont font_;
};

} // ui
} // ramen

#endif
