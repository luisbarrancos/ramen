// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/imageio/reader.hpp>

#include <boost/tuple/tuple.hpp>

namespace ramen
{
namespace imageio
{
class RAMEN_API multichannel_reader_t : public reader_t
{
  public:
    multichannel_reader_t(const boost::filesystem::path& p);

    const std::vector<std::string>& channel_list() const { return channels_; }

    void add_channel_name(const std::string& c);

    bool has_extra_channels() const { return has_extra_channels_; }

    void read_image(
        const image::image_view_t&              view,
        const math::box2i_t&                    crop,
        int                                     subsample,
        const boost::tuple<int, int, int, int>& channels) const
    {
        do_read_image(view, crop, subsample, channels);
    }

  private:
    void do_read_image(
        const image::image_view_t& view,
        const math::box2i_t&       crop,
        int                        subsample) const override;

    virtual void do_read_image(
        const image::image_view_t&              view,
        const math::box2i_t&                    crop,
        int                                     subsample,
        const boost::tuple<int, int, int, int>& channels) const = 0;

  protected:
    std::vector<std::string> channels_;
    bool                     has_extra_channels_;
};

}  // namespace imageio
}  // namespace ramen
