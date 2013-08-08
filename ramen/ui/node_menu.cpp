// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/ui/node_menu.hpp>

#include<QMenu>
#include<QAction>

namespace ramen
{
namespace ui
{

node_menu_t::node_menu_t( const core::string8_t& name) : name_( name), menu_( 0)
{
    menu_ = new QMenu( name.c_str());
}

void node_menu_t::add_submenu( const core::string8_t& name)
{
    submenus_.push_back( menu_->addMenu( name.c_str()));
}

void node_menu_t::add_action( const core::string8_t& submenu, QAction *action)
{
    std::vector<QMenu*>::iterator menu_it = find_submenu( submenu);

    if( menu_it == submenus_.end())
    {
        submenus_.push_back( menu_->addMenu( QString( submenu.c_str())));
        menu_it = submenus_.end() - 1;
    }

    (*menu_it)->addAction( action);
}

std::vector<QMenu*>::iterator node_menu_t::find_submenu( const core::string8_t& name)
{
    for( std::vector<QMenu*>::iterator it( submenus_.begin()); it != submenus_.end(); ++it)
    {

        if( name == (*it)->title().toStdString().c_str())
            return it;
    }

    return submenus_.end();
}

} // namespace
} // namespace
