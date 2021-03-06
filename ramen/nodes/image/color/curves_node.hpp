// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/pointop_node.hpp>

#include <OpenEXR/ImathColor.h>

namespace ramen
{
namespace image
{
class curves_node_t : public pointop_node_t
{
  public:
    static const node_metaclass_t& curves_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    curves_node_t();

  protected:
    curves_node_t(const curves_node_t& other)
      : pointop_node_t(other)
    {
    }
    void operator=(const curves_node_t&);

  private:
    node_t* do_clone() const override { return new curves_node_t(*this); }

    void add_curve_param(
        const std::string&   name,
        const std::string&   id,
        const Imath::Color3c col = Imath::Color3c(255, 255, 255));

    void do_create_params() override;

    void do_process(
        const image::const_image_view_t& src,
        const image::image_view_t&       dst,
        const render::context_t&         context) override;
};

}  // namespace image
}  // namespace ramen
