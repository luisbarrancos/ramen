// Copyright (c) 2010 Esteban Tovagliari

#include<ramen/nodes/image/base_layer_node.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace image
{

base_layer_node_t::base_layer_node_t() : node_t() {}

base_layer_node_t::base_layer_node_t( const base_layer_node_t& other) : node_t( other) {}

void base_layer_node_t::do_calc_format( const render::context_t& context)
{
    node_t *in = 0;

    if( get_value<int>( param( "mode")))
        in = input( 1);
    else
        in = input( 0);

    set_format( in->format());
    set_aspect_ratio( in->aspect_ratio());
    set_proxy_scale( in->proxy_scale());
}

void base_layer_node_t::do_recursive_process( const render::context_t& context)
{
    // for layer nodes, we can render and composite
    // one layer at a time. So we render the inputs on
    // demand in the process method of each derived class.
    // It's a bit more work, but it's also more memory efficient
    // and we can also avoid rendering a layer if it's not
    // visible i.e. opacity == 0
    alloc_image();
    process( context);
}

void base_layer_node_t::render_input( std::size_t i, const render::context_t& context)
{
    RAMEN_ASSERT( input( i) && "Base layer node, null input node in render input");
    input( i)->recursive_process( context);
}

void base_layer_node_t::release_input_image( std::size_t i)
{
    RAMEN_ASSERT( input( i) && "Base layer node, null input node in release input image");
    input( i)->release_image();
}

} // namespace
} // namespace