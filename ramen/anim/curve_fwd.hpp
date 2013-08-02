// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_ANIM_CURVE_FWD_HPP
#define RAMEN_ANIM_CURVE_FWD_HPP

#include<ramen/config.hpp>

#include<ramen/math/point2.hpp>

namespace ramen
{
namespace anim
{

class float_key_t;
class float_curve_t;

template<class T>
class shape_key_t;

typedef shape_key_t<math::point2f_t> shape_key2f_t;

template<class P>
class shape_curve_t;

typedef shape_curve_t<math::point2f_t> shape_curve2f_t;

} // anim
} // ramen

#endif
