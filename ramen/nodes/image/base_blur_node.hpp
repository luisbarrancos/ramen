// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image/areaop_node.hpp>

namespace ramen
{
namespace image
{
class base_blur_node_t : public areaop_node_t
{
  public:
    enum blur_border_mode
    {
        border_black = 0,
        border_repeat = 1,
        border_reflect = 2
    };

    enum blur_channels_mode
    {
        channels_rgba = 0,
        channels_rgb = 1,
        channels_alpha = 2
    };

    base_blur_node_t();

  protected:
    base_blur_node_t(const base_blur_node_t& other)
      : areaop_node_t(other)
    {
    }
    void operator=(const base_blur_node_t&);

    bool expand_defined() const override;

    Imath::V2f adjust_blur_size(const Imath::V2f& size, int subsample) const;
    Imath::V2i round_blur_size(const Imath::V2f& size) const;

    void copy_src_image(
        int                 in,
        const Imath::Box2i& area,
        blur_border_mode    bmode);
};

}  // namespace image
}  // namespace ramen
