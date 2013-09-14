// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_NODES_VIEW_NODES_VIEW_TABS_HPP
#define	RAMEN_UI_NODES_VIEW_NODES_VIEW_TABS_HPP

#include<ramen/ui/nodes_view/nodes_view_tabs_fwd.hpp>

#include<QTabWidget>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{
namespace ui
{

class nodes_view_tabs_t : public QTabWidget
{
    Q_OBJECT

public:

    nodes_view_tabs_t( QWidget *parent = 0);

    void add_view( nodes::composite_node_t *n);

    void update_state();

private Q_SLOTS:

    void delete_tab( int index);
};

} // ui
} // ramen

#endif
