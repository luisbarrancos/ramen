// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_INSPECTOR_PANEL_FACTORY_HPP
#define	RAMEN_UI_INSPECTOR_PANEL_FACTORY_HPP

#include<map>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/ui/inspector/panel_fwd.hpp>

namespace ramen
{
namespace ui
{

class panel_factory_t
{
public:

    panel_factory_t();
    ~panel_factory_t();

    typedef std::map<node_t*,panel_t*>::iterator iterator;

    iterator begin()	{ return panels_.begin();}
    iterator end()		{ return panels_.end();}

    iterator create_panel( node_t *n);
    void delete_panel( node_t *n);

private:

    // non-copyable
    panel_factory_t( const panel_factory_t&);
    panel_factory_t& operator=( const panel_factory_t&);

    std::map<node_t*,panel_t*> panels_;
    std::map<node_t*,panel_t*>::iterator current_;
};

} // ui
} // ramen

#endif
