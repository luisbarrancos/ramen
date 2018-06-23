// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_LAYER_NODE_HPP
#define RAMEN_IMAGE_LAYER_NODE_HPP

#include <ramen/nodes/image/base_layer_node.hpp>

namespace ramen
{
namespace image
{
class layer_node_t : public base_layer_node_t
{
public:
    static const node_metaclass_t& layer_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    layer_node_t();

protected:
    layer_node_t(const layer_node_t& other)
    : base_layer_node_t(other)
    {
    }
    void operator=(const layer_node_t&);

private:
    node_t* do_clone() const override { return new layer_node_t(*this); }

    void do_create_params() override;

    void do_calc_bounds(const render::context_t& context) override;

    void         do_process(const render::context_t& context) override;
    virtual void do_process_mult_min_overlay_mix(const render::context_t& context);
};

}  // namespace
}  // namespace

#endif
