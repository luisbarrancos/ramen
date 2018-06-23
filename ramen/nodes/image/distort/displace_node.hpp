// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_DISPLACE_NODE_HPP
#define RAMEN_IMAGE_DISPLACE_NODE_HPP

#include <ramen/nodes/image/distort_node.hpp>

namespace ramen
{
namespace image
{
class displace_node_t : public distort_node_t
{
public:
    static const node_metaclass_t& displace_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    displace_node_t();

protected:
    displace_node_t(const displace_node_t& other)
    : distort_node_t(other)
    {
    }
    void operator=(const displace_node_t&);

private:
    node_t* do_clone() const override { return new displace_node_t(*this); }

    void do_create_params() override;

    bool do_is_identity() const override;

    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_inputs_interest(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
};

}  // namespace
}  // namespace

#endif
