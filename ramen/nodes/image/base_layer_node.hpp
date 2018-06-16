// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_BASE_LAYER_NODE_HPP
#define RAMEN_IMAGE_BASE_LAYER_NODE_HPP

#include <ramen/nodes/image_node.hpp>

namespace ramen
{
namespace image
{
class RAMEN_API base_layer_node_t : public image_node_t
{
public:
    base_layer_node_t();

protected:
    base_layer_node_t(const base_layer_node_t& other);
    void operator=(const base_layer_node_t&);

    void render_input(std::size_t i, const render::context_t& context);
    void release_input_image(std::size_t i);

private:
    void do_calc_format(const render::context_t& context) override;

    void do_recursive_process(const render::context_t& context) override;
};

}  // image
}  // ramen

#endif
