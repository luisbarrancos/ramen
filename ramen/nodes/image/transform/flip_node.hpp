// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_FLIP_NODE_HPP
#define RAMEN_IMAGE_FLIP_NODE_HPP

#include <ramen/nodes/image_node.hpp>

#include <OpenEXR/ImathMatrix.h>

namespace ramen
{
namespace image
{
class flip_node_t : public image_node_t
{
public:
    static const node_metaclass_t& flip_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    flip_node_t();

protected:
    flip_node_t(const flip_node_t& other)
    : image_node_t(other)
    {
    }
    void operator=(const flip_node_t&);

private:
    node_t* do_clone() const override { return new flip_node_t(*this); }

    void do_create_params() override;

    void calc_transform_matrix();

    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_inputs_interest(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;

    Imath::M33d xform_, inv_xform_;
};

}  // namespace
}  // namespace

#endif
