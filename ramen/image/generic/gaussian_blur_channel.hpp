// Copyright (c) 2011 Esteban Tovagliari
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
void gaussian_blur_channel(
    const const_channel_view_t&       src,
    const boost::gil::gray32f_view_t& tmp,
    const channel_view_t&             dst,
    float                             stddevx,
    float                             stddevy);

void gaussian_blur_gray(
    const boost::gil::gray32fc_view_t& src,
    const boost::gil::gray32f_view_t&  tmp,
    const boost::gil::gray32f_view_t&  dst,
    float                              stddevx,
    float                              stddevy);

}  // namespace generic
}  // namespace image
}  // namespace ramen
