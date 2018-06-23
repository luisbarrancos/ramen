// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_OCIO_COLORSPACE_NODE_HPP
#define RAMEN_OCIO_COLORSPACE_NODE_HPP

#include <ramen/nodes/image/pointop_node.hpp>

#include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;

namespace ramen
{
namespace image
{
class ocio_colorspace_node_t : public pointop_node_t
{
public:
    static const node_metaclass_t& ocio_colorspace_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    ocio_colorspace_node_t();

protected:
    ocio_colorspace_node_t(const ocio_colorspace_node_t& other)
    : pointop_node_t(other)
    {
    }
    void operator=(const ocio_colorspace_node_t&);

private:
    node_t* do_clone() const override { return new ocio_colorspace_node_t(*this); }

    void do_create_params() override;

    void do_process(const image::const_image_view_t& src,
                    const image::image_view_t&       dst,
                    const render::context_t&         context) override;
};

}  // image
}  // ramen

#endif
