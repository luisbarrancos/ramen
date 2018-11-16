// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <OpenEXR/ImathMatrix.h>

#include <ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{
namespace generic
{
void convolve33(const const_image_view_t& src, const image_view_t& dst, const Imath::M33f& k);

}  // namespace
}  // namespace
}  // namespace

