// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_TURBULENT_DISPLACE_NODE_HPP
#define RAMEN_IMAGE_TURBULENT_DISPLACE_NODE_HPP

#include <ramen/nodes/image/distort_node.hpp>

namespace ramen
{
namespace image
{
class turbulent_displace_node_t : public distort_node_t
{
public:
    static const node_metaclass_t&  turbulent_displace_node_metaclass();
    const node_metaclass_t* metaclass() const override;

    turbulent_displace_node_t();

protected:
    turbulent_displace_node_t(const turbulent_displace_node_t& other)
    : distort_node_t(other)
    {
    }
    void operator=(const turbulent_displace_node_t&);

private:
    node_t* do_clone() const override { return new turbulent_displace_node_t(*this); }

    void do_create_params() override;

    bool do_is_identity() const override;

    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_inputs_interest(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
};

}  // namespace
}  // namespace

#endif
