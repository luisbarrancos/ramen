// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/animated_param.hpp>

#include <OpenEXR/ImathColor.h>

#include <QPointer>
#include <QToolButton>

#ifndef Q_MOC_RUN
#include <ramen/ui/widgets/color.hpp>
#include <ramen/ui/widgets/param_spinbox_fwd.hpp>
#include <ramen/ui/widgets/color_button_fwd.hpp>
#include <ramen/ui/widgets/eyedropper_button_fwd.hpp>
#endif

namespace ramen
{
class RAMEN_API color_param_t : public animated_param_t
{
    Q_OBJECT

  public:
    explicit color_param_t(const std::string& name);

    void set_default_value(const Imath::Color4f& x);

    poly_param_value_t value_at_frame(float frame) const override;

    void set_value(const Imath::Color4f& x, change_reason reason = user_edited);
    void set_value_at_frame(
        const Imath::Color4f& x,
        float                 frame,
        change_reason         reason = user_edited);

    bool is_rgba() const { return is_rgba_; }
    void set_is_rgba(bool b) { is_rgba_ = b; }

  protected:
    color_param_t(const color_param_t& other);
    void operator=(const color_param_t& other);

  private:
    void private_init();

    param_t* do_clone() const override { return new color_param_t(*this); }

    void do_create_tracks(anim::track_t* parent) override;

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    void do_update_widgets() override;
    void do_enable_widgets(bool e) override;

    bool is_rgba_;

    QWidget* do_create_widgets() override;

    void set_component_value_from_slot();

    QPointer<ui::param_spinbox_t>     input0_, input1_, input2_, input3_;
    QPointer<ui::color_button_t>      button_;
    QPointer<ui::eyedropper_button_t> eyedropper_;

  private Q_SLOTS:

    void value_changed(double value);
    void spinbox_pressed();
    void spinbox_dragged(double value);
    void spinbox_released();
    void eyedropper_color_picked(const ramen::ui::color_t& c);
    void color_button_pressed();
};

}  // namespace ramen
