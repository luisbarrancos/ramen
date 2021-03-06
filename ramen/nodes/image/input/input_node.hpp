// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image_node.hpp>

#include <boost/optional.hpp>
#include <memory>
#include <boost/tuple/tuple.hpp>

#include <ramen/filesystem/path.hpp>

#include <ramen/nodes/image/input/input_clip.hpp>

#include <ramen/nodes/image/input/image_input_command_fwd.hpp>

namespace ramen
{
namespace image
{
class input_node_t : public image_node_t
{
  public:
    static const node_metaclass_t& image_input_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    input_node_t();
    input_node_t(
        const boost::filesystem::path& path,
        bool                           sequence,
        const boost::filesystem::path& from_dir);

    // set / get for undo
    boost::tuple<int, int, int, int> get_channels() const;
    void set_channels(const boost::tuple<int, int, int, int>& c);

    void set_channels(
        const std::string& red,
        const std::string& green,
        const std::string& blue,
        const std::string& alpha);

    float get_aspect_param_value() const;
    void  set_aspect_param_value(float a);

    // add colorspace here

  protected:
    input_node_t(const input_node_t& other);
    void operator=(const input_node_t&);

  private:
    friend class undo::image_input_command_t;

    node_t* do_clone() const override { return new input_node_t(*this); }

    void do_create_params() override;
    void create_image_params(
        const boost::filesystem::path& p = boost::filesystem::path());
    void create_more_params();

    void param_changed(param_t* p, param_t::change_reason reason);

    void                 do_set_frame(float t) override;
    boost::optional<int> map_frame_to_sequence(float t) const;

    bool do_is_valid() const override;

    void do_calc_hash_str(const render::context_t& context) override;

    void do_calc_format(const render::context_t& context) override;
    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_defined(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;

    // serialization
    virtual void do_read(
        const serialization::yaml_node_t& node,
        const std::pair<int, int>&        version);
    void do_write(serialization::yaml_oarchive_t& out) const override;

    // paths
    void convert_relative_paths(
        const boost::filesystem::path& old_base,
        const boost::filesystem::path& new_base) override;
    void make_paths_absolute() override;
    void make_paths_relative() override;

    // misc
    const std::vector<input_clip_t>& clips() const { return clips_; }
    std::vector<input_clip_t>&       clips() { return clips_; }

    int  index_for_proxy_level(int proxy_level) const;
    void create_reader(int proxy_level);
    void create_reader(
        int                            proxy_level,
        const boost::filesystem::path& from_dir);

    void file_picked(
        const boost::filesystem::path& p,
        int                            level,
        bool                           sequence,
        bool                           relative);

    static const int num_proxy_levels;

    Imath::Box2i                                    real_defined_;
    std::vector<input_clip_t>                       clips_;
    std::vector<std::shared_ptr<movieio::reader_t>> readers_;
};

}  // namespace image
}  // namespace ramen
