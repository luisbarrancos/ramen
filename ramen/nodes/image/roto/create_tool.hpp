// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image/roto/tool.hpp>

#include <memory>

#include <ramen/nodes/image/roto/shape_fwd.hpp>
#include <ramen/nodes/image/roto/triple.hpp>

namespace ramen
{
namespace roto
{
class create_tool_t : public tool_t
{
  public:
    create_tool_t(image::roto_node_t& parent);

    void end_active() override;

    void draw_overlay(const ui::paint_event_t& event) const override;

    void key_press_event(const ui::key_press_event_t& event) override;

    void mouse_press_event(const ui::mouse_press_event_t& event) override;
    void mouse_drag_event(const ui::mouse_drag_event_t& event) override;
    void mouse_release_event(const ui::mouse_release_event_t& event) override;

  private:
    void close_shape(bool success = true);

    shape_t* pick_shape(
        const ui::mouse_press_event_t& event,
        int&                           span_index,
        float&                         t);

    std::auto_ptr<roto::shape_t> new_shape_;
    roto::triple_t*              point_;
};

}  // namespace roto
}  // namespace ramen
