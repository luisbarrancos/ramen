// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_UNSHARP_MASK_NODE_HPP
#define RAMEN_IMAGE_UNSHARP_MASK_NODE_HPP

#include <ramen/nodes/image/areaop_node.hpp>

namespace ramen
{
namespace image
{
class unsharp_mask_node_t : public areaop_node_t
{
public:
    static const node_metaclass_t&  unsharp_mask_node_metaclass();
    const node_metaclass_t* metaclass() const override;

    unsharp_mask_node_t();

protected:
    unsharp_mask_node_t(const unsharp_mask_node_t& other)
    : areaop_node_t(other)
    {
    }
    void operator=(const unsharp_mask_node_t&);

private:
    node_t* do_clone() const override { return new unsharp_mask_node_t(*this); }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;

    void get_expand_radius(int& hradius, int& vradius) const override;
};

}  // namespace
}  // namespace

#endif
