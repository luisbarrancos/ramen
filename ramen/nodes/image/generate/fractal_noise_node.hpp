// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_FRACTAL_NOISE_NODE_HPP
#define RAMEN_IMAGE_FRACTAL_NOISE_NODE_HPP

#include <ramen/nodes/image/generator_node.hpp>

namespace ramen
{
namespace image
{
class fractal_noise_node_t : public generator_node_t
{
public:
    static const node_metaclass_t& fractal_noise_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    fractal_noise_node_t();

    bool use_cache(const render::context_t& context) const override { return true; }

protected:
    fractal_noise_node_t(const fractal_noise_node_t& other)
    : generator_node_t(other)
    {
    }
    void operator=(const fractal_noise_node_t&);

private:
    node_t* do_clone() const override { return new fractal_noise_node_t(*this); }

    void do_create_params() override;

    void do_process(const render::context_t& context) override;
};

}  // namespace
}  // namespace

#endif
