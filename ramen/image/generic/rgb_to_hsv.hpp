// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/gil/extension/algorithm/tbb/tbb_transform.hpp>

#include <cassert>

#include <ramen/image/color.hpp>

namespace ramen
{
namespace image
{
namespace generic
{
void convert_rgb_to_hsv(const const_image_view_t& src, const image_view_t& dst)
{
    assert(src.width() == dst.width());
    assert(src.height() == dst.height());

    pixel_t (*rgb_to_hsv_fun)(const pixel_t&) = rgb_to_hsv;
    boost::gil::tbb_transform_pixels(src, dst, rgb_to_hsv_fun);
}

void convert_hsv_to_rgb(const const_image_view_t& src, const image_view_t& dst)
{
    assert(src.width() == dst.width());
    assert(src.height() == dst.height());

    pixel_t (*hsv_to_rgb_fun)(const pixel_t&) = hsv_to_rgb;
    boost::gil::tbb_transform_pixels(src, dst, hsv_to_rgb_fun);
}

}  // namespace generic
}  // namespace image
}  // namespace ramen
