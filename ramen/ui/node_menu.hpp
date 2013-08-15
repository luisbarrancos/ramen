// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_NODE_MENU_HPP
#define	RAMEN_UI_NODE_MENU_HPP

#include<ramen/core/string8.hpp>

#include<vector>

#include<ramen/core/string8.hpp>

class QAction;
class QMenu;

namespace ramen
{
namespace ui
{

class node_menu_t
{
public:

    explicit node_menu_t( core::string8_t name);

    const core::string8_t& name() const { return name_;}
    QMenu *menu() { return menu_;}

    std::vector<QMenu*>& submenus() { return submenus_;}

    void add_submenu( const core::string8_t& name);

    void add_action( const core::string8_t& submenu, QAction *action);

private:

    std::vector<QMenu*>::iterator find_submenu( const core::string8_t& name);

    core::string8_t name_;
    QMenu *menu_;
    std::vector<QMenu*> submenus_;
};

} // ui
} // ramen

#endif
