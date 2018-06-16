// Copyright (c) 2011 Esteban Tovagliari

#ifndef RAMEN_IMAGE_BLUR_MASK_NODE_HPP
#define RAMEN_IMAGE_BLUR_MASK_NODE_HPP

#include <ramen/nodes/image/base_blur_node.hpp>

namespace ramen
{
namespace image
{
class blur_mask_node_t : public base_blur_node_t
{
public:
    static const node_metaclass_t&  blur_mask_node_metaclass();
    const node_metaclass_t* metaclass() const override;

    blur_mask_node_t();

protected:
    blur_mask_node_t(const blur_mask_node_t& other)
    : base_blur_node_t(other)
    {
    }
    void operator=(const blur_mask_node_t&);

private:
    node_t* do_clone() const override { return new blur_mask_node_t(*this); }

    void do_create_params() override;

    bool do_is_identity() const override;

    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_inputs_interest(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;

    Imath::V2f get_max_blur_radius() const;
};

}  // namespace
}  // namespace

#endif
