// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/image/generic/projective_warp.hpp>

#include <ramen/image/generic/samplers.hpp>

namespace ramen
{
namespace image
{
namespace generic
{
void projective_warp_nearest(
    const Imath::Box2i&       src_area,
    const const_image_view_t& src,
    const Imath::Box2i&       dst_area,
    const image_view_t&       dst,
    const matrix3_t&          xform,
    const matrix3_t&          inv_xform)
{
    point_sampler_t s(src_area, src);
    projective_warp<point_sampler_t>(s, dst_area, dst, xform, inv_xform);
}

}  // namespace generic
}  // namespace image
}  // namespace ramen
