// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_FLOAT2_PARAM_HPP
#define RAMEN_FLOAT2_PARAM_HPP

#include <ramen/params/proportional_param.hpp>

#include <OpenEXR/ImathVec.h>

#include <QPointer>

#include <ramen/ui/widgets/param_spinbox_fwd.hpp>

namespace ramen
{
class RAMEN_API float2_param_t : public proportional_param_t
{
    Q_OBJECT

public:
    explicit float2_param_t(const std::string& name);

    void set_default_value(const Imath::V2f& x);

    poly_param_value_t value_at_frame(float frame) const override;

    void set_value(const Imath::V2f& x, change_reason reason = user_edited);
    void set_value_at_frame(const Imath::V2f& x, float frame, change_reason reason = user_edited);

    void set_absolute_value(const Imath::V2f& x, change_reason reason = user_edited);
    void set_absolute_value_at_frame(const Imath::V2f& x,
                                     float             frame,
                                     change_reason     reason = user_edited);

    Imath::V2f derive(float time) const;
    Imath::V2f integrate(float time1, float time2) const;

protected:
    float2_param_t(const float2_param_t& other);
    void operator=(const float2_param_t& other);

private:
    void private_init();

    param_t* do_clone() const override { return new float2_param_t(*this); }

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    void do_format_changed(const Imath::Box2i& new_domain,
                           float               aspect,
                           const Imath::V2f&   proxy_scale) override;

    void do_update_widgets() override;
    void do_enable_widgets(bool e) override;

    QWidget* do_create_widgets() override;

    void calc_proportional_factors();
    void set_component_value_from_slot();

    QPointer<ui::param_spinbox_t> input0_, input1_;

private Q_SLOTS:

    void value_changed(double value);
    void spinbox_pressed();
    void spinbox_dragged(double value);
    void spinbox_released();
};

}  // namespace

#endif
