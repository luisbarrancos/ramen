// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/config.hpp>

namespace ramen
{
namespace util
{
template <class T, class Bit> bool test_flag(T flags, Bit bit)
{
    return flags & bit;
}

template <class T, class Bit> void set_flag(T& flags, Bit bit, bool value)
{
    if (value)
        flags |= bit;
    else
        flags &= ~bit;
}

}  // namespace util
}  // namespace ramen
