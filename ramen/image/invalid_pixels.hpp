// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{
void count_invalid_pixels(
    const image_view_t& img,
    std::size_t&        nan_pixels,
    std::size_t&        neg_pixels);

}  // namespace image
}  // namespace ramen
