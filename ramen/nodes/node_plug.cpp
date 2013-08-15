// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/node_plug.hpp>

#include<algorithm>

#include<boost/range/algorithm/find_if.hpp>

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace
{

struct match_output_connection_by_port
{
    match_output_connection_by_port( node_t *node, int num)
    {
        node_ = node;
        num_ = num;
    }

    bool operator()( const node_output_plug_t::connection_t& c) const
    {
        if( boost::get<0>( c) == node_ && boost::get<1>( c) == num_)
            return true;

        return false;
    }

    node_t *node_;
    int num_;
};

} // unnamed

node_plug_t::node_plug_t( const core::string8_t& ui_label,
                          const color::color3c_t& color)
{
    color_ = color;
    ui_label_ = ui_label;
}

node_plug_t::node_plug_t( const node_plug_t& other) : ui_label_( other.ui_label_),
                                                      color_( other.color_)
{
}

node_plug_t& node_plug_t::operator=( const node_plug_t& other)
{
    ui_label_ = other.ui_label_;
    color_ = other.color_;
}

node_input_plug_t::node_input_plug_t( const core::string8_t& ui_label,
                                      const color::color3c_t& color,
                                      bool optional) : node_plug_t( ui_label, color)
{
    input_ = 0;
    optional_ = optional;
}

node_input_plug_t::node_input_plug_t( const node_input_plug_t& other) : node_plug_t( other)
{
    input_ = 0;
    optional_ = other.optional();
}

node_input_plug_t& node_input_plug_t::operator=( const node_input_plug_t& other)
{
    input_ = 0;
    optional_ = other.optional();
    return *this;
}

node_output_plug_t::node_output_plug_t( node_t *parent,
                                        const core::string8_t& ui_label,
                                        const color::color3c_t& color) : node_plug_t( ui_label, color)
{
    RAMEN_ASSERT( parent);

    parent_ = parent;
}

node_output_plug_t::node_output_plug_t( const node_output_plug_t& other) : node_plug_t( other)
{
    parent_ = 0;
}

void node_output_plug_t::add_output( node_t *n, int port)
{
    RAMEN_ASSERT( boost::range::find_if( connections_,
                                        match_output_connection_by_port( n, port)) == connections_.end());
    connections_.push_back( boost::tuples::make_tuple( n, port));
}

void node_output_plug_t::remove_output( node_t *n, int port)
{
    iterator it( boost::range::find_if( connections_,
                                        match_output_connection_by_port( n, port)));

    if( it != connections_.end())
        connections_.erase( it);
}

} // ramen
