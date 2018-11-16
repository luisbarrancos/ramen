// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/animated_param.hpp>

#include <OpenEXR/ImathColor.h>

namespace ramen
{
class RAMEN_API curve_param_t : public animated_param_t
{
    Q_OBJECT

  public:
    explicit curve_param_t(const std::string& name);

    const Imath::Color3c& color() const { return color_; }
    void                  set_color(const Imath::Color3c& c) { color_ = c; }

  protected:
    curve_param_t(const curve_param_t& other);
    void operator=(const curve_param_t& other);

  private:
    void private_init();

    param_t* do_clone() const override { return new curve_param_t(*this); }

    void do_create_tracks(anim::track_t* parent) override;

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    Imath::Color3c color_;
};

}  // namespace ramen
