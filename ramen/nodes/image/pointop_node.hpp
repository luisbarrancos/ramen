// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image_node.hpp>

#include <ramen/gil/extension/algorithm/tbb/tbb_transform.hpp>

namespace ramen
{
namespace image
{
class RAMEN_API pointop_node_t : public image_node_t
{
  public:
    pointop_node_t();

    bool use_cache(const render::context_t& context) const override
    {
        return false;
    }

  protected:
    pointop_node_t(const pointop_node_t& other);
    void operator=(const pointop_node_t&);

  private:
    void         do_process(const render::context_t& context) override;
    virtual void do_process(
        const image::const_image_view_t& src,
        const image::image_view_t&       dst,
        const render::context_t&         context) = 0;
};

}  // namespace image
}  // namespace ramen
