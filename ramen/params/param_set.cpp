// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/param_set.hpp>

#include<boost/range/algorithm/for_each.hpp>
#include<boost/bind.hpp>

namespace ramen
{
namespace params
{

param_set_t::param_set_t( node_t *n) : node_( n) {}

param_set_t::param_set_t( const param_set_t& other) : composite_param_t( other)
{
    node_ = 0;
    boost::range::for_each( params(), boost::bind( &param_t::set_parent, _1, this));
}

void param_set_t::for_each_param( const boost::function<void ( param_t*)>& f)
{
    apply_function( f);
}

} // params
} // ramen
