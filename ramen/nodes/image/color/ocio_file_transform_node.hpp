// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_OCIO_FILE_TRANSFORM_NODE_HPP
#define RAMEN_OCIO_FILE_TRANSFORM_NODE_HPP

#include <ramen/nodes/image/pointop_node.hpp>

namespace ramen
{
namespace image
{
class ocio_file_transform_node_t : public pointop_node_t
{
public:
    static const node_metaclass_t& ocio_file_transform_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    ocio_file_transform_node_t();

protected:
    ocio_file_transform_node_t(const ocio_file_transform_node_t& other)
    : pointop_node_t(other)
    {
    }
    void operator=(const ocio_file_transform_node_t&);

private:
    node_t* do_clone() const override { return new ocio_file_transform_node_t(*this); }

    void do_create_params() override;

    bool do_is_valid() const override;

    void do_process(const image::const_image_view_t& src,
                    const image::image_view_t&       dst,
                    const render::context_t&         context) override;
};

}  // image
}  // ramen

#endif
