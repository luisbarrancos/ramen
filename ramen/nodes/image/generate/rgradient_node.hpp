// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/generator_node.hpp>

namespace ramen
{
namespace image
{
class rgradient_node_t : public generator_node_t
{
  public:
    static const node_metaclass_t& rgradient_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    rgradient_node_t();

  protected:
    rgradient_node_t(const rgradient_node_t& other)
      : generator_node_t(other)
    {
    }
    void operator=(const rgradient_node_t&);

  private:
    node_t* do_clone() const override { return new rgradient_node_t(*this); }

    void do_create_params() override;
    void do_create_manipulators() override;

    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_defined(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
};

}  // namespace image
}  // namespace ramen
