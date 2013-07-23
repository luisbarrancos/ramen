// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UTIL_STRING_HPP
#define RAMEN_UTIL_STRING_HPP

#include<string>

#include<ramen/core/name.hpp>

namespace ramen
{
namespace util
{

bool is_string_valid_identifier( const std::string& str);
bool is_string_valid_identifier( const core::name_t& str);

void increment_string_number( std::string& str);

} // util
} // ramen

#endif
