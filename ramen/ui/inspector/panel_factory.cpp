// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/inspector/panel_factory.hpp>

#include<boost/bind.hpp>

#include<ramen/nodes/node.hpp>

#include<ramen/ui/inspector/panel.hpp>

namespace ramen
{
namespace ui
{

panel_factory_t::panel_factory_t() {}

panel_factory_t::~panel_factory_t()
{
    //RAMEN_ASSERT( panels_.empty());
}

panel_factory_t::iterator panel_factory_t::create_panel( node_t *n)
{
    RAMEN_ASSERT( n != 0);

    iterator it = panels_.find( n);

    if( it == panels_.end())
    {
        do_create_panel( n);
        it = panels_.find( n);
    }

    return it;
}

void panel_factory_t::delete_panel( node_t *n)
{
    std::map<node_t*, panel_t*>::iterator it( panels_.find( n));

    if( it != panels_.end())
    {
        delete it->second;
        panels_.erase( n);
    }
}

void panel_factory_t::do_create_panel( node_t *n)
{
    panel_t *panel = new panel_t( n);
    panels_[ n] = panel;
    boost::signals2::connection c = n->deleted.connect( boost::bind( &panel_factory_t::delete_panel, this, _1));
    panel->set_connection( c);
}

} // ui
} // ramen
