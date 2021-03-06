// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/imageio/reader.hpp>

namespace ramen
{
namespace imageio
{
class jpg_reader_t : public reader_t
{
  public:
    jpg_reader_t(const boost::filesystem::path& p);

  private:
    void do_read_image(
        const image::image_view_t& view,
        const math::box2i_t&       crop,
        int                        subsample) const override;
};

}  // namespace imageio
}  // namespace ramen
