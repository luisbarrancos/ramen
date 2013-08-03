// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_FLOAT2_PARAM_HPP
#define	RAMEN_FLOAT2_PARAM_HPP

#include<ramen/params/proportional_param.hpp>

#include<ramen/math/vector2.hpp>

#include<QPointer>

#include<ramen/ui/widgets/param_spinbox_fwd.hpp>

namespace ramen
{

class RAMEN_API float2_param_t : public proportional_param_t
{
    Q_OBJECT

public:

    float2_param_t();

    void set_default_value( const math::vector2f_t& x);

    virtual poly_param_value_t value_at_frame( float frame) const;

    void set_value( const math::vector2f_t& x, change_reason reason = user_edited);
    void set_value_at_frame( const math::vector2f_t& x,
                             float frame,
                             change_reason reason = user_edited);

    void set_absolute_value( const math::vector2f_t& x, change_reason reason = user_edited);
    void set_absolute_value_at_frame( const math::vector2f_t& x,
                                      float frame,
                                      change_reason reason = user_edited);

    math::vector2f_t derive( float time) const;
    math::vector2f_t integrate( float time1, float time2) const;

protected:

    float2_param_t( const float2_param_t& other);
    void operator=( const float2_param_t& other);

private:

    void private_init();

    virtual param_t *do_clone() const { return new float2_param_t( *this);}

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    //virtual void do_read( const serialization::yaml_node_t& node);
    //virtual void do_write( serialization::yaml_oarchive_t& out) const;

    virtual void do_format_changed( const math::box2i_t& new_domain,
                                    float aspect,
                                    const math::vector2f_t& proxy_scale);

    virtual void do_update_widgets();
    virtual void do_enable_widgets( bool e);

    virtual QWidget *do_create_widgets();

    void calc_proportional_factors();
    void set_component_value_from_slot();

    QPointer<ui::param_spinbox_t> input0_, input1_;

private Q_SLOTS:

    void value_changed( double value);
    void spinbox_pressed();
    void spinbox_dragged( double value);
    void spinbox_released();
};

} // ramen

#endif

