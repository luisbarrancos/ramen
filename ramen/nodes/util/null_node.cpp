// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/node.hpp>

#include<ramen/params/float_param.hpp>

namespace ramen
{

class null_node_t : public node_t
{
public:

    static const node_info_t& null_node_info();
    virtual const node_info_t *node_info() const;

    null_node_t()
    {
        set_name( "null");
        add_input_plug( "front", false, ui::palette_t::instance().color("front plug"), "Front");
        add_output_plug();
    }

    virtual bool use_cache( const render::context_t& context) const { return false;}

protected:

    null_node_t( const null_node_t& other) : node_t(other) {}

private:

    void operator=( const null_node_t&);

    node_t *do_clone() const { return new null_node_t(*this);}

    virtual bool do_is_identity() const { return true;}
};


// factory
node_t *create_null_node() { return new null_node_t();}

const node_info_t *null_node_t::node_info() const { return &null_node_info();}

const node_info_t& null_node_t::null_node_info()
{
    static bool inited( false);
    static node_info_t info;

    if( !inited)
    {
        info.id = core::name_t( "image.builtin.null");
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
