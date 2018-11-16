// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/ui/viewer/image_view/image_strategy.hpp>

namespace ramen
{
namespace ui
{
namespace viewer
{
class empty_image_strategy_t : public image_strategy_t
{
public:
    empty_image_strategy_t(const Imath::Box2i& display_window);

    bool update_pixels(const image::buffer_t& pixels,
                       const Imath::Box2i&    display_window,
                       const Imath::Box2i&    data_window) override;

    void draw() const override;

    boost::optional<Imath::Color4f> color_at(const Imath::V2i& p) const override;
};

}  // viewer
}  // ui
}  // ramen

