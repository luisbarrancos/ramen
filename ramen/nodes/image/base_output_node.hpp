// Copyright (c) 2010 Esteban Tovagliari.
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image_node.hpp>
#include <ramen/nodes/node_output_interface.hpp>

#include <OpenEXR/ImathColor.h>

namespace ramen
{
namespace image
{
class RAMEN_API base_output_node_t
: public image_node_t
, public node_output_interface
{
public:
    base_output_node_t();

    void process_and_write(const render::context_t& context) override;

    // by default, output nodes doesn't use the cache.
    bool use_cache(const render::context_t& context) const override { return false; }

protected:
    base_output_node_t(const base_output_node_t& other);
    void operator=(const base_output_node_t&);

    void do_process(const render::context_t& context) override;

    const Imath::Box2i input_defined() const { return input_defined_; }

private:
    virtual void write(const render::context_t& context) = 0;

    Imath::Box2i input_defined_;
};

}  // image
}  // ramen

