// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/pointop_node.hpp>

namespace ramen
{
namespace image
{
class set_channels_node_t : public pointop_node_t
{
  public:
    static const node_metaclass_t& set_channels_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    set_channels_node_t();

  protected:
    set_channels_node_t(const set_channels_node_t& other)
      : pointop_node_t(other)
    {
    }
    void operator=(const set_channels_node_t&);

  private:
    node_t* do_clone() const override { return new set_channels_node_t(*this); }

    void do_create_params() override;

    void do_calc_bounds(const render::context_t& context) override;

    void do_process(
        const image::const_image_view_t& src,
        const image::image_view_t&       dst,
        const render::context_t&         context) override;

    void copy_channel(
        const image::const_image_view_t& src,
        int                              src_ch,
        const image::image_view_t&       dst,
        int                              dst_ch);
};

}  // namespace image
}  // namespace ramen
