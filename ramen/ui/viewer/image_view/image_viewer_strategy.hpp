// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/ui/viewer/viewer_strategy.hpp>

// avoid a moc error, when including boost
#ifndef Q_MOC_RUN
#include <memory>

#include <ramen/nodes/image_node.hpp>

#include <ramen/ocio/gl_display_manager.hpp>

#include <ramen/ui/viewer/image_view/image.hpp>
#include <ramen/ui/viewport.hpp>
#include <ramen/ui/events.hpp>
#endif

class QWidget;

namespace ramen
{
namespace ui
{
namespace viewer
{
class image_view_toolbar_t;

class image_viewer_strategy_t : public viewer_strategy_t
{
    Q_OBJECT

  public:
    image_viewer_strategy_t(viewer_context_t* p);
    ~image_viewer_strategy_t() override;

    void init() override;

    QWidget* toolbar() override;

    bool can_display_node(node_t* n) const override;

    void set_active_node(node_t* n, bool process = false) override;
    void set_context_node(node_t* n, bool process = false) override;

    // options
    int  proxy_level() const;
    int  subsample() const;
    bool mblur_active() const;

    void autoupdate_changed() override;
    void view_mode_changed() override;

    void frame_changed() override;

    // ocio
    void display_transform_changed() override;
    void exposure_changed() override;
    void gamma_changed() override;

    // coordinate conversions & projections
    float aspect_ratio() const;
    void  set_aspect_ratio(float asp);

    float pixel_scale() const;

    Imath::V2f screen_to_world(const Imath::V2i& p) const;
    Imath::V2i world_to_screen(const Imath::V2f& p) const;

    Imath::Box2f screen_to_world(const Imath::Box2i& b) const;
    Imath::Box2i world_to_screen(const Imath::Box2f& b) const;

    Imath::V2f screen_to_world_dir(const Imath::V2f& v) const;

    void save_projection();
    void restore_projection();
    void set_screen_projection();

    // resize & paint
    void resize(int w, int h) override;
    void paint() override;

    // event handling
    void enter_event(QEvent* event) override;
    void leave_event(QEvent* event) override;

    void key_press_event(QKeyEvent* event) override;
    void key_release_event(QKeyEvent* event) override;

    void mouse_move_event(QMouseEvent* event) override;
    void mouse_press_event(QMouseEvent* event) override;
    void mouse_release_event(QMouseEvent* event) override;

    void wheel_event(QWheelEvent* event) override;

    Imath::Color4f color_at(int x, int y) const override;
    void           pick_colors_in_box(
                  const Imath::Box2i&                        b,
                  std::function<void(const Imath::Color4f&)> f) const override;

    virtual void center_image();
    virtual void frame_image();

  public Q_SLOTS:

    void change_resolution(int index);
    void change_proxy_level(int index);
    void change_channels(int index);

    void aspect_toggle(bool state);
    void mblur_toggle(bool state);
    void checks_toggle(bool state);
    void roi_toggle(bool state);
    void overlay_toggle(bool state);

  private:
    void do_begin_active_view() override;
    void do_end_active_view() override;

    void active_node_changed();
    void active_overlay_changed();

    void context_node_changed();
    void context_overlay_changed();

    void call_node_changed();

    node_t* visible_node();

    void render_visible_node();
    void load_texture(image_node_t* n);
    void clear_texture();

    void draw_image();

    bool           first_image_loaded_;
    ui::viewport_t viewport_;

    boost::signals2::connection active_connection_;
    boost::signals2::connection active_overlay_connection_;
    boost::signals2::connection context_connection_;
    boost::signals2::connection context_overlay_connection_;

    std::auto_ptr<ocio::gl_display_manager_t> display_;
    image_t                                   image_;

    bool pending_update_;

    std::auto_ptr<image_view_toolbar_t> toolbar_;

    int  subsample_;
    int  proxy_level_;
    bool overlay_;
    bool checks_;
    bool mblur_;
    bool aspect_;

    // event handling
    mouse_press_event_t   press_event_;
    mouse_move_event_t    move_event_;
    mouse_drag_event_t    drag_event_;
    mouse_release_event_t release_event_;
    mouse_enter_event_t   enter_event_;
    mouse_leave_event_t   leave_event_;
    key_press_event_t     key_press_event_;
    key_release_event_t   key_release_event_;
    paint_event_t         paint_event_;

    bool       scroll_mode_;
    bool       zoom_mode_;
    Imath::V2f zoom_center_;
    bool       event_accepted_by_node_;
    float      aspect_ratio_;
};

}  // namespace viewer
}  // namespace ui
}  // namespace ramen
