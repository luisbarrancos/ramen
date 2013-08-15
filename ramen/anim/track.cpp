// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/anim/track.hpp>

#include<boost/bind.hpp>

#include<ramen/assert.hpp>

#include<ramen/anim/curve.hpp>

namespace ramen
{
namespace anim
{

track_t::track_t() { init();}

track_t::track_t( core::string8_t name)
{
    set_name( name);
    init();
}

track_t::track_t( core::string8_t name, const any_curve_ptr_t& curve) : curve_( curve)
{
    set_name( name);
    init();
}

void track_t::init()
{
    parent_ = 0;
    color_ = color::color3c_t( 255, 255, 255);
}

track_t *track_t::child( int i)
{
    RAMEN_ASSERT( i >= 0);
    RAMEN_ASSERT( i < num_children());
    return &children_[i];
}

void track_t::add_child( BOOST_RV_REF( core::auto_ptr_t<track_t>) t)
{
    t->set_parent( this);
    children_.push_back( t);
}

void track_t::clear_children() { children_.clear();}

int track_t::row() const
{
    for( int i = 0; i < parent()->num_children(); ++i)
    {
        if( &( parent()->children()[i]) == this)
            return i;
    }

    return 0;
}

const core::string8_t& track_t::name() const
{
    RAMEN_ASSERT( !name_.empty());
    return name_;
}

void track_t::set_name( core::string8_t name)
{
    RAMEN_ASSERT( !name.empty());

    name_.swap( name);
}

const core::string8_t& track_t::full_name() const	{ return full_name_;}
const core::string8_t& track_t::curve_name() const	{ return curve_name_;}

void track_t::make_full_names()
{
    full_name_ = name_;

    for( int i = 0; i < num_children(); ++i)
        child( i)->do_make_full_names();

    for_each_track_depth_first( this, boost::bind( &track_t::make_curve_name, _1));
}

void track_t::do_make_full_names()
{
    RAMEN_ASSERT( full_name_.empty());

    if( track_t *p = parent())
        full_name_ = p->full_name() + "." + name();
    else
        full_name_ = name();

    for( int i = 0; i < num_children(); ++i)
        child( i)->do_make_full_names();
}

void track_t::make_curve_name()
{
    // TODO: replace find_first_of
    std::string tmp( full_name_.c_str());
    std::string::size_type pos = tmp.find_first_of( '.');

    if( pos != std::string::npos)
        curve_name_ = core::string8_t( tmp.c_str(), pos + 1, full_name_.size() - pos + 1);
}

void track_t::set_color( const color::color3c_t& col)
{
    color_ = col;
}

void track_t::notify()
{
    RAMEN_ASSERT( curve());
    changed( curve().get());
}

} // anim
} // ramen
