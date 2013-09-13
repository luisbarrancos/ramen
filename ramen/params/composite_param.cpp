// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/composite_param.hpp>

#include<boost/bind.hpp>
#include<boost/foreach.hpp>

#include<boost/range/algorithm/for_each.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/params/param_set.hpp>

#include<ramen/anim/track.hpp>

namespace ramen
{
namespace params
{

composite_param_t::composite_param_t() : param_t() {}
composite_param_t::composite_param_t( const composite_param_t& other) : param_t( other),
                                                                        params_( other.params_)
{
    boost::range::for_each( params(), boost::bind( &param_t::set_parent, _1, this));
}

void composite_param_t::do_init()
{
    boost::range::for_each( params(), boost::bind( &param_t::init, _1));
}

param_t *composite_param_t::do_clone() const
{
    return new composite_param_t( *this);
}

void composite_param_t::do_add_param( param_t *p)
{
    p->set_parent( this);

    param_set_t *ps = param_set();
    if( ps)
    {
        try
        {
            ps->find( p->id());
            throw core::runtime_error( core::make_string( "Duplicated param id found: ", p->id().c_str()));
        }
        catch( core::runtime_error& e) {}
    }
    else
    {
        if( find( p->id()))
            throw core::runtime_error( core::make_string( "Duplicated param id found: ", p->id().c_str()));
    }

    params().push_back( p);
}

bool composite_param_t::empty() const
{
    return params_.empty();
}

void composite_param_t::clear()
{
    params_.clear();
}

const param_t *composite_param_t::find( const core::name_t& id) const
{
    composite_param_t& self = const_cast<composite_param_t&>( *this);
    return self.find( id);
}

param_t *composite_param_t::find( const core::name_t& id)
{
    BOOST_FOREACH( param_t& p, params())
    {
        if( p.id() == id)
            return &p;

        if( composite_param_t *cp = dynamic_cast<composite_param_t*>( &p))
        {
            param_t *q = cp->find( id);

            if( q)
                return q;
        }
    }

    return 0;
}

void composite_param_t::do_create_tracks( anim::track_t *parent)
{
    /*
    if( create_track_)
    {
        core::auto_ptr_t<anim::track_t> t( new anim::track_t( ui_label()));
        boost::range::for_each( params(), boost::bind( &param_t::create_tracks, _1, t.get()));

        if( t->num_children() != 0)
            parent->add_child( boost::move( t));
    }
    else
    */
        boost::range::for_each( params(), boost::bind( &param_t::create_tracks, _1, parent));
}

void composite_param_t::do_convert_relative_paths( const boost::filesystem::path& old_base, const boost::filesystem::path& new_base)
{
    boost::range::for_each( params_, boost::bind( &param_t::convert_relative_paths, _1, old_base, new_base));
}

void composite_param_t::do_make_paths_absolute()
{
    boost::range::for_each( params_, boost::bind( &param_t::make_paths_absolute, _1));
}

void composite_param_t::do_make_paths_relative()
{
    boost::range::for_each( params(), boost::bind( &param_t::make_paths_relative, _1));
}

// util
void composite_param_t::do_apply_function( const boost::function<void ( param_t*)>& f)
{
    boost::range::for_each( params(), boost::bind( &param_t::apply_function, _1, f));
}

} // params
} // ramen
