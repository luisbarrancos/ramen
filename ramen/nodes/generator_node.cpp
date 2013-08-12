// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/generator_node.hpp>

#include<ramen/params/image_format_param.hpp>

namespace ramen
{

core::name_t generator_node_t::g_format( "format");

generator_node_t::generator_node_t() : node_t()
{
    add_output_plug();
}

generator_node_t::generator_node_t( const generator_node_t& other) : node_t( other) {}

void generator_node_t::do_create_params()
{
    std::auto_ptr<image_format_param_t> p( new image_format_param_t());
    p->set_name( "Format");
    p->set_id( g_format);
    add_param( p);
}

void generator_node_t::do_calc_format( const render::context_t& context)
{
    image::format_t f( get_value<image::format_t>( param( g_format)));
    math::box2i_t area( f.area());
    --area.max.x;
    --area.max.y;
    set_format( area);
    set_aspect_ratio( f.aspect);
    set_proxy_scale( math::vector2f_t( 1, 1));
}

void generator_node_t::do_calc_bounds( const render::context_t& context)
{
    set_bounds( format());
}

void generator_node_t::do_calc_defined( const render::context_t& context)
{
    set_defined( math::intersect( bounds(), interest()));
}

void generator_node_t::do_calc_hash_str( const render::context_t& context)
{
    node_t::do_calc_hash_str( context);
    hash_generator() << context.subsample;
}

} // ramen
