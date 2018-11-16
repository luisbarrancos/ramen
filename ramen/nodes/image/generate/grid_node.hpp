// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/generator_node.hpp>

namespace ramen
{
namespace image
{
class grid_node_t : public generator_node_t
{
  public:
    static const node_metaclass_t& grid_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    grid_node_t();

  protected:
    grid_node_t(const grid_node_t& other)
      : generator_node_t(other)
    {
    }
    void operator=(const grid_node_t&);

  private:
    node_t* do_clone() const override { return new grid_node_t(*this); }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;
};

}  // namespace image
}  // namespace ramen
