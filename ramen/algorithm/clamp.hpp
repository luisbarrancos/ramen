// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_ALGORITHM_CLAMP_HPP
#define RAMEN_ALGORITHM_CLAMP_HPP

#include <ramen/config.hpp>

#include <cassert>

namespace ramen
{
template<class T>
inline T clamp(T x, T lo, T hi)
{
    assert(lo <= hi);

    if (x < lo)
        return lo;

    if (x > hi)
        return hi;

    return x;
}

}  // algorithm

#endif
