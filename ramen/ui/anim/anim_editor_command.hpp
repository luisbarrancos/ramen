// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/undo/command.hpp>

#include <map>
#include <set>

#include <memory>

#include <ramen/nodes/node_fwd.hpp>

#include <ramen/anim/any_curve.hpp>
#include <ramen/anim/float_curve.hpp>
#include <ramen/anim/shape_curve.hpp>

#include <ramen/ui/anim/track_model.hpp>

namespace ramen
{
namespace undo
{
class anim_editor_command_t : public command_t
{
  public:
    anim_editor_command_t(
        node_t*                                   node,
        const std::shared_ptr<ui::track_model_t>& model);

    bool empty() const override;

    void add_track(anim::track_t* t);
    void call_notify_for_tracks();

    void add_node(node_t* n);
    void notify_nodes();

    void undo() override;
    void redo() override;

  protected:
    void swap_curves();

    typedef std::map<anim::track_t*, anim::any_curve_t> map_type;
    typedef map_type::iterator                          iterator;

    std::shared_ptr<ui::track_model_t> model_;
    std::set<node_t*>                  nodes_;
    map_type                           curves_;
};

}  // namespace undo
}  // namespace ramen
