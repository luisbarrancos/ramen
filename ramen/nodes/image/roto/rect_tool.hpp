// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image/roto/tool.hpp>

namespace ramen
{
namespace roto
{
class rect_tool_t : public tool_t
{
  public:
    rect_tool_t(image::roto_node_t& parent);

    void draw_overlay(const ui::paint_event_t& event) const override;

    void mouse_press_event(const ui::mouse_press_event_t& event) override;
    void mouse_drag_event(const ui::mouse_drag_event_t& event) override;
    void mouse_release_event(const ui::mouse_release_event_t& event) override;

  private:
    bool         creating_;
    Imath::Box2f box_;
};

}  // namespace roto
}  // namespace ramen
