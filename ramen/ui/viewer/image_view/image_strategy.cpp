// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/ui/viewer/image_view/image_strategy.hpp>

namespace ramen
{
namespace ui
{
namespace viewer
{
image_strategy_t::image_strategy_t(
    const Imath::Box2i& display_window,
    const Imath::Box2i& data_window,
    GLenum              texture_unit)
{
    texture_unit_ = texture_unit;
    data_window_ = data_window;
    display_window_ = display_window;
}

image_strategy_t::~image_strategy_t() {}

int image_strategy_t::width() const
{
    if (!data_window().isEmpty())
        return data_window().size().x + 1;

    return 0;
}

int image_strategy_t::height() const
{
    if (!data_window().isEmpty())
        return data_window().size().y + 1;

    return 0;
}

bool image_strategy_t::update_pixels(
    const image::buffer_t& pixels,
    const Imath::Box2i&    display_window,
    const Imath::Box2i&    data_window)
{
    return false;
}

}  // namespace viewer
}  // namespace ui
}  // namespace ramen
