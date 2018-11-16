// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/generator_node.hpp>

namespace ramen
{
namespace image
{
class checkerboard_node_t : public generator_node_t
{
  public:
    static const node_metaclass_t& checkerboard_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    checkerboard_node_t();

  protected:
    checkerboard_node_t(const checkerboard_node_t& other)
      : generator_node_t(other)
    {
    }
    void operator=(const checkerboard_node_t&);

  private:
    node_t* do_clone() const override { return new checkerboard_node_t(*this); }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;
};

}  // namespace image
}  // namespace ramen
