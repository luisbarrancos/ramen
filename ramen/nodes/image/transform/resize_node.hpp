// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_RESIZE_NODE_HPP
#define RAMEN_IMAGE_RESIZE_NODE_HPP

#include <ramen/nodes/image_node.hpp>

namespace ramen
{
namespace image
{
class resize_node_t : public image_node_t
{
public:
    static const node_metaclass_t&  resize_node_metaclass();
    const node_metaclass_t* metaclass() const override;

    resize_node_t();

protected:
    resize_node_t(const resize_node_t& other);
    void operator=(const resize_node_t&);

private:
    node_t* do_clone() const override { return new resize_node_t(*this); }

    void do_create_params() override;

    bool do_is_identity() const override;

    void do_calc_format(const render::context_t& context) override;
    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_inputs_interest(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;

    Imath::V2f scale_;
};

}  // namespace
}  // namespace

#endif
