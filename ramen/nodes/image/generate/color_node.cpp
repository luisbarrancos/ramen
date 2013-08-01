// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/image/generator_node.hpp>

#include<ramen/params/color_param.hpp>

namespace ramen
{
namespace
{

core::name_t g_color( "color");

} // unnamed

class color_node_t : public generator_node_t
{
public:

    static const node_info_t& color_node_info();
    virtual const node_info_t *node_info() const;

    color_node_t() : generator_node_t()
    {
        set_name( "color");
    }

protected:

    color_node_t( const color_node_t& other) : generator_node_t( other)
    {
    }

private:

    void operator=( const color_node_t&);

    virtual node_t *do_clone() const { return new color_node_t( *this);}

    virtual void do_create_params()
    {
        generator_node_t::do_create_params();

        std::auto_ptr<color_param_t> c( new color_param_t( "Color"));
        c->set_id( g_color);
        c->set_default_value( color::rgba_colorf_t( 0, 0, 0, 0));
        add_param( c);
    }

    virtual void do_process( const render::context_t& context)
    {
        color::rgba_colorf_t c( get_value<color::rgba_colorf_t>( param( g_color)));
        image::pixel_t p;
        boost::gil::get_color( p, boost::gil::red_t())   = c.r;
        boost::gil::get_color( p, boost::gil::green_t()) = c.g;
        boost::gil::get_color( p, boost::gil::blue_t())  = c.b;
        boost::gil::get_color( p, boost::gil::alpha_t()) = c.a;
        boost::gil::fill_pixels( image_view(), p);
    }
};

// factory
node_t *create_color_node() { return new color_node_t();}

const node_info_t *color_node_t::node_info() const { return &color_node_info();}

const node_info_t& color_node_t::color_node_info()
{
    static bool inited( false);
    static node_info_t info;

    if( !inited)
    {
        info.id = core::name_t( "image.builtin.color");
        info.major_version = 1;
        info.minor_version = 0;
        info.menu = "Image";
        info.submenu = "Input";
        info.menu_item = "Color";
        info.create = &create_color_node;
        inited = true;
    }

    return info;
}

static bool registered = node_factory_t::instance().register_node( color_node_t::color_node_info());

} // ramen
