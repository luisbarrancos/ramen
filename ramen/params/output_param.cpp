// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/output_param.hpp>

namespace ramen
{
namespace params
{

output_param_t::output_param_t()
{
}

output_param_t::output_param_t( const output_param_t& other) : param_t( other)
{
}

param_t *output_param_t::do_clone() const
{
    return new output_param_t( *this);
}

} // params
} // ramen
