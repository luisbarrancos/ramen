// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{
namespace generic
{
void apply_gamma(const const_image_view_t& src, const image_view_t& dst, float gamma);
void apply_log(const const_image_view_t& src, const image_view_t& dst);
void apply_pow10(const const_image_view_t& src, const image_view_t& dst);

}  // namespace
}  // namespace
}  // namespace

