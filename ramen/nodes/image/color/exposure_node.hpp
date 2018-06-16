// Copyright (c) 2010 Esteban Tovagliari

#ifndef RAMEN_IMAGE_EXPOSURE_NODE_HPP
#define RAMEN_IMAGE_EXPOSURE_NODE_HPP

#include <ramen/nodes/image/pointop_node.hpp>

namespace ramen
{
namespace image
{
class exposure_node_t : public pointop_node_t
{
public:
    static const node_metaclass_t&  exposure_node_metaclass();
    const node_metaclass_t* metaclass() const override;

    exposure_node_t();

protected:
    exposure_node_t(const exposure_node_t& other)
    : pointop_node_t(other)
    {
    }
    void operator=(const exposure_node_t&);

private:
    node_t* do_clone() const override { return new exposure_node_t(*this); }

    void do_create_params() override;

    bool do_is_identity() const override;

    void do_process(const image::const_image_view_t& src,
                            const image::image_view_t&       dst,
                            const render::context_t&         context) override;
};

}  // namespace
}  // namespace

#endif
