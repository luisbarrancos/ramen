// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_LUM_KEY_NODE_HPP
#define RAMEN_LUM_KEY_NODE_HPP

#include <ramen/nodes/image/pointop_node.hpp>

namespace ramen
{
namespace image
{
class lum_key_node_t : public pointop_node_t
{
public:
    static const node_metaclass_t&  lum_key_node_metaclass();
    const node_metaclass_t* metaclass() const override;

    lum_key_node_t();

protected:
    lum_key_node_t(const lum_key_node_t& other)
    : pointop_node_t(other)
    {
    }
    void operator=(const lum_key_node_t&);

private:
    node_t* do_clone() const override { return new lum_key_node_t(*this); }

    void do_create_params() override;

    void do_process(const image::const_image_view_t& src,
                            const image::image_view_t&       dst,
                            const render::context_t&         context) override;
};

}  // namespace
}  // namespace

#endif