// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_ROTO_NODE_HPP
#define RAMEN_ROTO_NODE_HPP

#include <ramen/nodes/image_node.hpp>

#include <ramen/nodes/image/roto/scene.hpp>
#include <ramen/nodes/image/roto/toolbar_fwd.hpp>

namespace ramen
{
class inspector_param_t;
class roto_shape_param_t;

namespace image
{
class roto_node_t : public image_node_t
{
public:
    static const node_metaclass_t& roto_node_metaclass();
    const node_metaclass_t*        metaclass() const override;

    roto_node_t();

    virtual std::auto_ptr<QWidget> create_toolbar();

    const roto::toolbar_t& toolbar() const;
    roto::toolbar_t&       toolbar();

    bool track_mouse() const override;

    // shapes
    std::auto_ptr<roto::shape_t> create_shape() const;
    std::auto_ptr<roto::shape_t> create_shape(const Imath::Box2f& b) const;
    std::auto_ptr<roto::shape_t> create_null() const;

    void                         add_shape(std::auto_ptr<roto::shape_t> s);
    std::auto_ptr<roto::shape_t> release_shape(roto::shape_t* s);

    const roto::scene_t& scene() const { return scene_; }
    roto::scene_t&       scene() { return scene_; }

    // selections
    void           deselect_all();
    roto::shape_t* selected();
    void           selection_changed();

    // params
    void for_each_param(const std::function<void(param_t*)>& f) override;

protected:
    roto_node_t(const roto_node_t& other);
    void operator=(const roto_node_t&);

private:
    node_t* do_clone() const override { return new roto_node_t(*this); }

    void do_create_params() override;
    void param_changed(param_t* p, param_t::change_reason reason);

    void do_create_manipulators() override;

    // time, anim & hash
    void do_set_frame(float f) override;
    void do_create_tracks(anim::track_t* parent) override;
    void do_update_widgets() override;

    void do_calc_hash_str(const render::context_t& context) override;

    // areas and processing

    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_inputs_interest(const render::context_t& context) override;

    void do_process(const render::context_t& context) override;
    void do_process_motion_blur(const render::context_t& context, const Imath::Box2i& area);

    // serialization
    void do_read(const serialization::yaml_node_t& node,
                 const std::pair<int, int>&        version) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    // util functions
    void init_shape(roto::shape_t& s) const;

    roto::scene_t scene_;

    roto::toolbar_t* toolbar_;

    roto_shape_param_t* shape_param_;
    inspector_param_t*  inspector_;

    Imath::Box2i       scene_bbox_;
    std::vector<float> frames_;
};

}  // namespace
}  // namespace

#endif
