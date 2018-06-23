// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_COLOR_NODE_HPP
#define RAMEN_COLOR_NODE_HPP

#include <ramen/nodes/image/generator_node.hpp>

namespace ramen
{
namespace image
{
class color_node_t : public generator_node_t
{
public:
    static const node_metaclass_t& color_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    color_node_t();

protected:
    color_node_t(const color_node_t& other)
    : generator_node_t(other)
    {
    }
    void operator=(const color_node_t&);

private:
    node_t* do_clone() const override { return new color_node_t(*this); }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;
};

}  // namespace
}  // namespace

#endif
