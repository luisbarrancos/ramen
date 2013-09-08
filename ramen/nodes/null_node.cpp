// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/null_node.hpp>

namespace ramen
{

null_node_t::null_node_t()
{
    set_name( "null");
    add_input_plug( "Front", ui::palette_t::instance().color("front plug"), false);
    add_output_plug();
}

null_node_t::null_node_t( const null_node_t& other) : node_t(other) {}

node_t *null_node_t::do_clone() const { return new null_node_t(*this);}

bool null_node_t::do_is_identity() const { return true;}

// factory
node_t *create_null_node() { return new null_node_t();}

const node_info_t *null_node_t::node_info() const { return &null_node_info();}

const node_info_t& null_node_t::null_node_info()
{
    static bool inited( false);
    static node_info_t info;

    if( !inited)
    {
        info.id = core::name_t( "builtin.null");
        info.major_version = 1;
        info.minor_version = 0;
        info.menu = "Image";
        info.submenu = "Util";
        info.menu_item = "Null";
        info.help = "No operation (Null)";
        info.create = &create_null_node;
        inited = true;
    }

    return info;
}

static bool registered = node_factory_t::instance().register_node( null_node_t::null_node_info());

} // ramen
