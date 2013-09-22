// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/viewer_node.hpp>

namespace ramen
{
namespace nodes
{

core::name_t g_viewer_node_id( "builtin.viewer");

viewer_node_t::viewer_node_t()
{
    set_name( "viewer");
    //add_input_plug( "Input", ui::palette_t::instance().color("front plug"), true);
    //add_output_plug();
}

viewer_node_t::viewer_node_t( const viewer_node_t& other) : node_t(other)
{
}

// factory
node_t *create_viewer_node()
{
    return new viewer_node_t();
}

node_t *create_viewer_node_gui()
{
    return new viewer_node_t();
}

const node_info_t *viewer_node_t::node_info() const
{
    return &viewer_node_info();
}

const node_info_t& viewer_node_t::viewer_node_info()
{
    static bool inited( false);
    static node_info_t info;

    if( !inited)
    {
        info.id = g_viewer_node_id;
        info.major_version = 1;
        info.minor_version = 0;
        info.help = "Viewer";
        info.create = &create_viewer_node;
        info.create_gui = &create_viewer_node_gui;
        inited = true;
    }

    return info;
}

static bool registered = node_factory_t::instance().register_node( viewer_node_t::viewer_node_info());

} // nodes
} // ramen
