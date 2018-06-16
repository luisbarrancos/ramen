// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_COPY_CHANNELS_NODE_HPP
#define RAMEN_IMAGE_COPY_CHANNELS_NODE_HPP

#include <ramen/nodes/image_node.hpp>

namespace ramen
{
namespace image
{
class copy_channels_node_t : public image_node_t
{
public:
    static const node_metaclass_t&  copy_channels_node_metaclass();
    const node_metaclass_t* metaclass() const override;

    copy_channels_node_t();

    bool use_cache(const render::context_t& context) const override { return false; }

protected:
    copy_channels_node_t(const copy_channels_node_t& other)
    : image_node_t(other)
    {
    }
    void operator=(const copy_channels_node_t&);

private:
    node_t* do_clone() const override { return new copy_channels_node_t(*this); }

    void do_create_params() override;

    void do_calc_bounds(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;

    void copy_channel(const image::const_image_view_t& src,
                      int                              src_ch,
                      const image::image_view_t&       dst,
                      int                              dst_ch);
};

}  // namespace
}  // namespace

#endif
