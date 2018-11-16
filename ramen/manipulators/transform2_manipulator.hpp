// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/manipulators/manipulator.hpp>

#include <ramen/params/transform2_param.hpp>

namespace ramen
{
class transform2_param_t;

class transform2_manipulator_t : public manipulator_t
{
  public:
    transform2_manipulator_t(param_t* param);

  private:
    void do_draw_overlay(const ui::paint_event_t& event) const override;

    bool do_key_press_event(const ui::key_press_event_t& event) override;
    void do_key_release_event(const ui::key_release_event_t& event) override;

    bool do_mouse_press_event(const ui::mouse_press_event_t& event) override;
    void do_mouse_drag_event(const ui::mouse_drag_event_t& event) override;
    void do_mouse_release_event(
        const ui::mouse_release_event_t& event) override;

    transform2_param_t* param_;
    bool                drag_center_;
    bool                drag_axes_;
    bool                dragx_, dragy_;
};

}  // namespace ramen
