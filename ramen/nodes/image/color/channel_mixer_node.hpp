// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/pointop_node.hpp>

namespace ramen
{
namespace image
{
class channel_mixer_node_t : public pointop_node_t
{
  public:
    static const node_metaclass_t& channel_mixer_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    channel_mixer_node_t();

  protected:
    channel_mixer_node_t(const channel_mixer_node_t& other)
      : pointop_node_t(other)
    {
    }
    void operator=(const channel_mixer_node_t&);

  private:
    node_t* do_clone() const override
    {
        return new channel_mixer_node_t(*this);
    }

    void do_create_params() override;

    void do_process(
        const image::const_image_view_t& src,
        const image::image_view_t&       dst,
        const render::context_t&         context) override;
};

}  // namespace image
}  // namespace ramen
