// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/areaop_node.hpp>

namespace ramen
{
namespace image
{
class percentile_filter_node_t : public areaop_node_t
{
  public:
    static const node_metaclass_t& percentile_filter_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    percentile_filter_node_t();

  protected:
    percentile_filter_node_t(const percentile_filter_node_t& other)
      : areaop_node_t(other)
    {
    }
    void operator=(const percentile_filter_node_t&);

  private:
    node_t* do_clone() const override
    {
        return new percentile_filter_node_t(*this);
    }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;

    void get_expand_radius(int& hradius, int& vradius) const override;
};

}  // namespace image
}  // namespace ramen
