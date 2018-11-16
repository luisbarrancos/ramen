// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/pointop_node.hpp>

namespace ramen
{
namespace image
{
class adjust_hsv_node_t : public pointop_node_t
{
public:
    static const node_metaclass_t& adjust_hsv_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    adjust_hsv_node_t();

protected:
    adjust_hsv_node_t(const adjust_hsv_node_t& other)
    : pointop_node_t(other)
    {
    }
    void operator=(const adjust_hsv_node_t&);

private:
    node_t* do_clone() const override { return new adjust_hsv_node_t(*this); }

    void do_create_params() override;

    void do_process(const image::const_image_view_t& src,
                    const image::image_view_t&       dst,
                    const render::context_t&         context) override;
};

}  // namespace
}  // namespace

