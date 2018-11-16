// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/areaop_node.hpp>

namespace ramen
{
namespace image
{
class alpha_ops_node_t : public areaop_node_t
{
  public:
    static const node_metaclass_t& alpha_ops_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    alpha_ops_node_t();

  protected:
    alpha_ops_node_t(const alpha_ops_node_t& other)
      : areaop_node_t(other)
    {
    }
    void operator=(const alpha_ops_node_t&);

    bool expand_defined() const override;

  private:
    node_t* do_clone() const override { return new alpha_ops_node_t(*this); }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;

    void get_expand_radius(int& hradius, int& vradius) const override;
};

}  // namespace image
}  // namespace ramen
