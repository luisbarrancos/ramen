// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/nodes/image/base_blur_node.hpp>

namespace ramen
{
namespace image
{
class blur_node_t : public base_blur_node_t
{
public:
    static const node_metaclass_t& blur_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    blur_node_t();

protected:
    blur_node_t(const blur_node_t& other)
    : base_blur_node_t(other)
    {
    }
    void operator=(const blur_node_t&);

private:
    node_t* do_clone() const override { return new blur_node_t(*this); }

    void do_create_params() override;

    bool do_is_identity() const override;

    void do_process(const render::context_t& context) override;

    void get_expand_radius(int& hradius, int& vradius) const override;
};

}  // namespace
}  // namespace

