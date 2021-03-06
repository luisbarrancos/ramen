// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image_node.hpp>

namespace ramen
{
namespace image
{
class crop_node_t : public image_node_t
{
  public:
    static const node_metaclass_t& crop_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    crop_node_t();

  protected:
    crop_node_t(const crop_node_t& other)
      : image_node_t(other)
    {
    }
    void operator=(const crop_node_t&);

  private:
    node_t* do_clone() const override { return new crop_node_t(*this); }

    void do_create_params() override;

    void do_calc_format(const render::context_t& context) override;
    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_inputs_interest(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
};

}  // namespace image
}  // namespace ramen
