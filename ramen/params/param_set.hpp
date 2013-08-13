// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAM_PARAM_SET_HPP
#define	RAMEN_PARAM_PARAM_SET_HPP

#include<ramen/params/param_set_fwd.hpp>

#include<map>

#include<boost/signals2/signal.hpp>

#include<ramen/containers/ptr_vector.hpp>

#include<ramen/params/param.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{

class composite_param_t;
class param_set_command_t;

/*!
\ingroup params
\brief A container for params, with some extra functionality
*/
class RAMEN_API param_set_t
{
public:

    param_set_t( node_t *n = 0);
    param_set_t( const param_set_t& other);

    ~param_set_t();

    const node_t *node() const    { return node_;}
    node_t *node()                { return node_;}
    void set_node( node_t *n)     { node_ = n;}

    void clear();
    bool empty() const { return params_.empty();}

    // iterator and ranges

    const containers::ptr_vector_t<param_t>& params() const    { return params_;}
    containers::ptr_vector_t<param_t>& params()                { return params_;}

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

    const param_t& find( const core::name_t& id) const;
    param_t& find( const core::name_t& id);

    boost::signals2::signal<void ( param_t*, param_t::change_reason)> param_changed;
    void notify_node();

    void begin_edit();
    void end_edit( bool notify = true);

    bool editing() const;

    const param_set_command_t *command() const;
    param_set_command_t	*command();

    bool is_command_empty() const;

    void add_command( param_t *p);

    bool autokey() const;

    void add_to_hash( hash::generator_t& hash_gen) const;

    // util
    void for_each_param( const boost::function<void ( param_t*)>& f);

    // serialization
    //void read( const serialization::yaml_node_t& node);
    //void write( serialization::yaml_oarchive_t& out) const;

    //void read_param( const serialization::yaml_node_t& node);

private:

    void do_add_param( param_t *p);

    node_t *node_;
    containers::ptr_vector_t<param_t> params_;
    core::auto_ptr_t<param_set_command_t> command_;
};

} // ramen

#endif
