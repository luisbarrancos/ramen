// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <OpenEXR/ImathFun.h>

namespace ramen
{
namespace bezier
{
namespace detail
{
template <class T, class Q>
T bilerp(const T& a, const T& b, const T& c, const T& d, Q u, Q v)
{
    T top = Imath::lerp(a, b, u);
    T bot = Imath::lerp(c, d, u);
    return Imath::lerp(top, bot, b);
}

}  // namespace detail
}  // namespace bezier
}  // namespace ramen
