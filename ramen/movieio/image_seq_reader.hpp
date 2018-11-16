// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/movieio/reader.hpp>

#include <memory>

#include <ramen/filesystem/path_sequence.hpp>

#include <ramen/imageio/reader.hpp>

namespace ramen
{
namespace movieio
{
class RAMEN_API image_seq_reader_t : public reader_t
{
public:
    image_seq_reader_t(const boost::filesystem::path& p, bool sequence);
    explicit image_seq_reader_t(const filesystem::path_sequence_t& seq);

    bool is_sequence() const override;
    int  start_frame() const override;
    int  end_frame() const override;
    int  pad() const override;

    bool has_extra_channels() const override { return has_extra_channels_; }

    virtual std::string format_string() const;
    virtual std::string string_for_current_frame() const;

private:
    void init();

    void do_set_frame(int frame) override;

    void do_read_frame(const image::image_view_t& view,
                       const math::box2i_t&       crop,
                       int                        subsample) const override;

    virtual void do_read_frame(const image::image_view_t&              view,
                               const math::box2i_t&                    crop,
                               int                                     subsample,
                               const boost::tuple<int, int, int, int>& channels) const;

    void get_sequence_info();

    void create_reader();

    filesystem::path_sequence_t        seq_;
    std::shared_ptr<imageio::reader_t> reader_;
    core::dictionary_t                 frame_info_;
    bool                               has_extra_channels_;
};

}  // movieio
}  // ramen

