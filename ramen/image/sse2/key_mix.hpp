// Copyright (c) 2010 Esteban Tovagliari
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
void key_mix(const const_image_view_t&   bg,
             const const_image_view_t&   fg,
             const const_channel_view_t& key,
             const image_view_t&         dst);

}  // namespace
}  // namespace
}  // namespace

