// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image_node.hpp>

namespace ramen
{
class transform_blur_node_t : public image_node_t
{
  public:
    static const node_metaclass_t& transform_blur_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    transform_blur_node_t();

  protected:
    transform_blur_node_t(const transform_blur_node_t& other)
      : image_node_t(other)
    {
    }
    void operator=(const transform_blur_node_t&);

  private:
    node_t* do_clone() const override
    {
        return new transform_blur_node_t(*this);
    }

    void do_create_params() override;

    void do_calc_bounds(const render::context_t& context) override;

    void do_calc_inputs_interest(const render::context_t& context) override;

    void         do_process(const render::context_t& context) override;
    virtual void do_process(
        const image::image_view_t& dst,
        const Imath::M33d&         xf,
        int                        border_mode);
};

}  // namespace ramen
