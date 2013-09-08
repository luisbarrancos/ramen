// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COLOR_BARS_HPP
#define RAMEN_COLOR_BARS_HPP

#include<ramen/config.hpp>

#include<ramen/math/box2.hpp>

#include<ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{

void make_color_bars( const image_view_t& view);
void make_color_bars( const image_view_t& view,
                      const math::box2i_t& domain,
                      const math::box2i_t& defined);

} // namespace
} // namespace

#endif
