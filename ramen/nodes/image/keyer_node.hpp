// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image_node.hpp>

#include <boost/optional.hpp>

namespace ramen
{
namespace image
{
class RAMEN_API keyer_node_t : public image_node_t
{
  public:
    keyer_node_t(bool add_mask_input = true);

    boost::optional<Imath::Color3f> sample_input(const Imath::V2i& p) const;
    void                            sample_input(
                                   const Imath::Box2i&          area,
                                   std::vector<Imath::Color3f>& colors) const;

  protected:
    keyer_node_t(const keyer_node_t& other);
    void operator=(const keyer_node_t&);

    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_defined(const render::context_t& context) override;

    void get_input_frame();
    void free_input_frame();

  private:
    Imath::Box2i    input_data_window_;
    image::buffer_t input_pixels_;
};

}  // namespace image
}  // namespace ramen
