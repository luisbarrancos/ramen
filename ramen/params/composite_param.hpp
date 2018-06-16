// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COMPOSITE_PARAM_HPP
#define RAMEN_COMPOSITE_PARAM_HPP

#include <ramen/config.hpp>

#include <ramen/params/param.hpp>

#include <memory>

#include <boost/ptr_container/ptr_vector.hpp>

#include <ramen/undo/command.hpp>

namespace ramen
{
class RAMEN_API composite_param_t : public param_t
{
    Q_OBJECT

public:
    composite_param_t();
    explicit composite_param_t(const std::string& name);

    void set_create_track(bool b) { create_track_ = b; }

    const boost::ptr_vector<param_t>& params() const { return params_; }
    boost::ptr_vector<param_t>&       params() { return params_; }

    template<class T>
    void add_param(std::auto_ptr<T> p)
    {
        do_add_param(p.release());
    }

    const param_t* find(const std::string& id) const;
    param_t*       find(const std::string& id);

protected:
    composite_param_t(const composite_param_t& other);
    void operator=(const composite_param_t& other);

    void create_widgets_inside_widget(QWidget* parent);

private:
    void     do_init() override;
    param_t* do_clone() const override { return new composite_param_t(*this); }

    void do_set_param_set(param_set_t* parent) override;

    void do_add_param(param_t* p);

    void do_set_frame(float frame) override;
    void do_evaluate(float frame) override;

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_update_widgets() override;
    void do_enable_widgets(bool e) override;

    void do_create_tracks(anim::track_t* parent) override;

    void do_format_changed(const Imath::Box2i& new_format,
                                   float               aspect,
                                   const Imath::V2f&   proxy_scale) override;

    // paths
    void do_convert_relative_paths(const boost::filesystem::path& old_base,
                                           const boost::filesystem::path& new_base) override;

    void do_make_paths_absolute() override;
    void do_make_paths_relative() override;

    // util
    void do_apply_function(const boost::function<void(param_t*)>& f) override;

    // serialization
    virtual void do_read(serialization::yaml_iarchive_t& node);
    void do_write(serialization::yaml_oarchive_t& out) const override;

    QWidget* do_create_widgets() override;

    boost::ptr_vector<param_t> params_;
    bool                       create_track_;
};

}  // namespace

#endif
