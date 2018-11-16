// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{
namespace sse2
{
void gaussian_blur_rgba(
    const const_image_view_t& src,
    const image_view_t&       tmp,
    const image_view_t&       dst,
    float                     std_devx,
    float                     std_devy);

}  // namespace sse2
}  // namespace image
}  // namespace ramen
