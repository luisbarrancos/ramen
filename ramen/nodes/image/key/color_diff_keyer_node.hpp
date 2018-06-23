// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_COLORDIFF_KEYER_NODE_HPP
#define RAMEN_COLORDIFF_KEYER_NODE_HPP

#include <ramen/nodes/image/keyer_node.hpp>

namespace ramen
{
namespace image
{
class color_diff_keyer_node_t : public keyer_node_t
{
public:
    static const node_metaclass_t& color_diff_keyer_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    color_diff_keyer_node_t();

    bool use_cache(const render::context_t& context) const override { return false; }

protected:
    color_diff_keyer_node_t(const color_diff_keyer_node_t& other)
    : keyer_node_t(other)
    {
    }
    void operator=(const color_diff_keyer_node_t&);

private:
    node_t* do_clone() const override { return new color_diff_keyer_node_t(*this); }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;
};

}  // namespace
}  // namespace

#endif
