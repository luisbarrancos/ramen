// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/imageio/writer.hpp>

namespace ramen
{
namespace imageio
{
class jpg_writer_t : public writer_t
{
  public:
    jpg_writer_t()
      : writer_t()
    {
    }

  private:
    void do_write_image(
        const boost::filesystem::path&   p,
        const image::const_image_view_t& view,
        const core::dictionary_t&        params) const override;
};

}  // namespace imageio
}  // namespace ramen
