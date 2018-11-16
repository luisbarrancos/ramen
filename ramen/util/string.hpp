// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <string>

namespace ramen
{
namespace util
{
bool is_string_valid_identifier(const std::string& str);

void increment_string_number(std::string& str);

}  // namespace util
}  // namespace ramen
