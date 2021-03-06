// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/base_layer_node.hpp>

namespace ramen
{
namespace image
{
class key_mix_layer_node_t : public base_layer_node_t
{
  public:
    static const node_metaclass_t& key_mix_layer_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    key_mix_layer_node_t();

  protected:
    key_mix_layer_node_t(const key_mix_layer_node_t& other)
      : base_layer_node_t(other)
    {
    }
    void operator=(const key_mix_layer_node_t&);

  private:
    node_t* do_clone() const override
    {
        return new key_mix_layer_node_t(*this);
    }

    void do_create_params() override;

    void do_calc_bounds(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
};

}  // namespace image
}  // namespace ramen
