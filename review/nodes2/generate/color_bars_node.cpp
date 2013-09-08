// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/generator_node.hpp>

#include<ramen/image/color_bars.hpp>

namespace ramen
{

class color_bars_node_t : public generator_node_t
{
public:

    static const node_info_t& color_bars_node_info();
    virtual const node_info_t *node_info() const;

    color_bars_node_t() : generator_node_t()
    {
        set_name( "color_bars");
    }

protected:

    color_bars_node_t( const color_bars_node_t& other) : generator_node_t( other) {}
    void operator=( const color_bars_node_t&);

private:

    virtual node_t *do_clone() const { return new color_bars_node_t( *this);}

    virtual void do_process( const render::context_t& context)
    {
        image::make_color_bars( image_view(), format(), defined());
    }
};

// factory
node_t *create_color_bars_node() { return new color_bars_node_t();}

const node_info_t *color_bars_node_t::node_info() const { return &color_bars_node_info();}

const node_info_t& color_bars_node_t::color_bars_node_info()
{
    static bool inited( false);
    static node_info_t info;

    if( !inited)
    {
        info.id = core::name_t( "image.builtin.color_bars");
        info.major_version = 1;
        info.minor_version = 0;
        info.menu = "Image";
        info.submenu = "Input";
        info.menu_item = "Color Bars";
        info.create = &create_color_bars_node;
        inited = true;
    }

    return info;
}

static bool registered = node_factory_t::instance().register_node( color_bars_node_t::color_bars_node_info());

} // ramen
