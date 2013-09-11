// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/curve_param.hpp>

#include<boost/bind.hpp>

#include<ramen/anim/track.hpp>

#include<ramen/ui/anim/anim_editor.hpp>

namespace ramen
{

curve_param_t::curve_param_t() : animated_param_t() { private_init();}

curve_param_t::curve_param_t( const curve_param_t& other) : animated_param_t( other) {}

void curve_param_t::private_init()
{
    add_curve( ui_label());
    color_ = color::color3c_t( 255, 255, 255);
}

void curve_param_t::do_create_tracks( anim::track_t *parent)
{
    core::auto_ptr_t<anim::track_t> t( new anim::track_t( ui_label(), &( curve( 0))));
    t->set_color( color_);
    t->changed.connect( boost::bind( &animated_param_t::anim_curve_changed, this, _1));
    parent->add_child( boost::move( t));
}

void curve_param_t::do_add_to_hash( hash::generator_t& hash_gen) const { hash_gen << curve().str();}

// serialization
/*
void curve_param_t::do_read( const serialization::yaml_node_t& node)
{
    serialization::yaml_node_t c( node.get_node( "curve"));
    curve().read( c);
}

void curve_param_t::do_write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "curve" << YAML::Value;
    curve().write( out);
}
*/

} // ramen