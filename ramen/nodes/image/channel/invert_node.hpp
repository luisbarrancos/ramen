// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_INVERT_NODE_HPP
#define RAMEN_IMAGE_INVERT_NODE_HPP

#include <ramen/nodes/image/pointop_node.hpp>

namespace ramen
{
namespace image
{
class invert_node_t : public pointop_node_t
{
public:
    static const node_metaclass_t& invert_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    invert_node_t();

protected:
    invert_node_t(const invert_node_t& other)
    : pointop_node_t(other)
    {
    }
    void operator=(const invert_node_t&);

private:
    node_t* do_clone() const override { return new invert_node_t(*this); }

    void do_create_params() override;

    void do_calc_bounds(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
    void do_process(const image::const_image_view_t& src,
                    const image::image_view_t&       dst,
                    const render::context_t&         context) override;
};

}  // namespace
}  // namespace

#endif
