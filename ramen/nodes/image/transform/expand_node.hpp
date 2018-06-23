// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_EXPAND_NODE_HPP
#define RAMEN_IMAGE_EXPAND_NODE_HPP

#include <ramen/nodes/image_node.hpp>

namespace ramen
{
namespace image
{
class expand_node_t : public image_node_t
{
public:
    static const node_metaclass_t& expand_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    expand_node_t();

    bool use_cache(const render::context_t& context) const override { return false; }

protected:
    expand_node_t(const expand_node_t& other)
    : image_node_t(other)
    {
    }
    void operator=(const expand_node_t&);

private:
    node_t* do_clone() const override { return new expand_node_t(*this); }

    void do_create_params() override;

    void do_calc_format(const render::context_t& context) override;
    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_inputs_interest(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
};

}  // namespace
}  // namespace

#endif
