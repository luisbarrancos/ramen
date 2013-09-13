// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAMS_COMPOSITE_PARAM_HPP
#define	RAMEN_PARAMS_COMPOSITE_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/params/param.hpp>

#include<ramen/containers/ptr_vector.hpp>

#include<ramen/undo/command.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API composite_param_t : public param_t
{
public:

    composite_param_t();

    const containers::ptr_vector_t<param_t>& params() const	{ return params_;}
    containers::ptr_vector_t<param_t>& params()				{ return params_;}

    typedef containers::ptr_vector_t<param_t>::const_iterator const_iterator;
    typedef containers::ptr_vector_t<param_t>::iterator iterator;

    const_iterator begin() const    { return params_.begin();}
    const_iterator end() const	    { return params_.end();}

    iterator begin()	{ return params_.begin();}
    iterator end()		{ return params_.end();}

    template<class T>
    void add_param( BOOST_RV_REF( core::auto_ptr_t<T>) p)
    {
        do_add_param( p.release());
    }

    bool empty() const;

    void clear();

    const param_t *find( const core::name_t& id) const;
    param_t *find( const core::name_t& id);

protected:

    composite_param_t( const composite_param_t& other);
    void operator=( const composite_param_t& other);

private:

    virtual void do_init();
    virtual param_t *do_clone() const;

    void do_add_param( param_t *p);

    virtual void do_create_tracks( anim::track_t *parent);

    // paths
    virtual void do_convert_relative_paths( const boost::filesystem::path& old_base,
                                            const boost::filesystem::path& new_base);

    virtual void do_make_paths_absolute();
    virtual void do_make_paths_relative();

    // util
    virtual void do_apply_function( const boost::function<void ( param_t*)>& f);

    containers::ptr_vector_t<param_t> params_;
};

} // params
} // ramen

#endif
