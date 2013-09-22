// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/input_param.hpp>

namespace ramen
{
namespace params
{

input_param_t::input_param_t()
{
}

input_param_t::input_param_t( const input_param_t& other) : param_t( other)
{
}

param_t *input_param_t::do_clone() const
{
    return new input_param_t( *this);
}

} // params
} // ramen
