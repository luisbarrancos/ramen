// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_NODES_VIEW_NODES_VIEW_TABS_HPP
#define	RAMEN_UI_NODES_VIEW_NODES_VIEW_TABS_HPP

#include<ramen/ui/nodes_view/nodes_view_tabs_fwd.hpp>

#include<vector>

#include<QTabWidget>

#include<ramen/core/string_fwd.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/ui/nodes_view/nodes_view_fwd.hpp>

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

    const nodes_view_t *active_view() const;
    nodes_view_t *active_view();

    void update_state();

private:

    void node_was_deleted( nodes::node_t *n);
    void node_was_renamed( nodes::node_t* n,
                           const core::string8_t& old_name,
                           const core::string8_t& new_name);

private Q_SLOTS:

    void delete_tab( int index);

private:

    std::vector<nodes_view_t*> views_;
    int active_view_;
};

} // ui
} // ramen

#endif
