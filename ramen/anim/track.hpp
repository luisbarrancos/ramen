// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_ANIM_TRACK_HPP
#define	RAMEN_ANIM_TRACK_HPP

#include<ramen/config.hpp>

#include<memory>

#include<boost/optional.hpp>
#include<boost/signals2/signal.hpp>

#include<ramen/core/string8.hpp>

#include<ramen/containers/ptr_vector.hpp>

#include<ramen/color/color3.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/anim/any_curve.hpp>

namespace ramen
{
namespace anim
{

class track_t
{
public:

    track_t();
    explicit track_t( const core::string8_t& name);
    track_t( const core::string8_t& name, const any_curve_ptr_t& curve);

    track_t *parent() const         { return parent_;}
    void set_parent( track_t *p)    { parent_ = p;}

    int num_children() const { return (int) children_.size();}

    track_t *child( int i);

    void add_child( BOOST_RV_REF( core::auto_ptr_t<track_t>) t);

    void clear_children();

    int row() const;

    // ranges
    const containers::ptr_vector_t<track_t>& children() const	{ return children_;}
    containers::ptr_vector_t<track_t>& children()              { return children_;}

    const core::string8_t& name() const;
    void set_name( const core::string8_t& name);

    const core::string8_t& full_name() const;
    void make_full_names();

    const core::string8_t& curve_name() const;

    bool is_leaf() const { return num_children() == 0;}

    const color::color3c_t& color() const { return color_;}
    void set_color( const color::color3c_t& col);

    // observer
    boost::signals2::signal<void ( any_curve_ptr_t&)> changed;

    void notify();

    const boost::optional<any_curve_ptr_t>& curve() const   { return curve_;}
    boost::optional<any_curve_ptr_t>& curve()               { return curve_;}

private:

    void init();

    void do_make_full_names();

    void make_curve_name();

    core::string8_t name_, full_name_, curve_name_;
    track_t *parent_;
    containers::ptr_vector_t<track_t> children_;
    boost::optional<any_curve_ptr_t> curve_;
    color::color3c_t color_;
};

template<class Fun>
void for_each_track_depth_first( track_t *t, Fun f)
{
    f( t);

    for( int i = 0; i < t->num_children(); ++i)
        for_each_track_depth_first( t->child( i), f);
}

template<class Fun>
void for_each_leaf_track( track_t *t, Fun f)
{
    if( t->is_leaf())
        f( t);
    else
    {
        for( int i = 0; i < t->num_children(); ++i)
            for_each_leaf_track( t->child( i), f);
    }
}

template<class Fun>
void for_each_curve( track_t *t, Fun f)
{
    if( t->is_leaf() && t->curve())
        f( t->curve().get());
    else
    {
        for( int i = 0; i < t->num_children(); ++i)
            for_each_curve( t->child( i), f);
    }
}

} // anim
} // ramen

#endif
