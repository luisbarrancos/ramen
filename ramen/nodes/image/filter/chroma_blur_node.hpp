// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/areaop_node.hpp>

namespace ramen
{
namespace image
{
class chroma_blur_node_t : public areaop_node_t
{
  public:
    static const node_metaclass_t& chroma_blur_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    chroma_blur_node_t();

  protected:
    chroma_blur_node_t(const chroma_blur_node_t& other)
      : areaop_node_t(other)
    {
    }
    void operator=(const chroma_blur_node_t&);

  private:
    node_t* do_clone() const override { return new chroma_blur_node_t(*this); }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;

    void get_expand_radius(int& hradius, int& vradius) const override;
};

}  // namespace image
}  // namespace ramen
