// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_WORLD_VIEW_HOOK_HPP
#define	RAMEN_UI_WORLD_VIEW_HOOK_HPP

#include<ramen/ui/world_view/world_view_hook_fwd.hpp>

#include<ramen/core/name_fwd.hpp>

#include<ramen/math/point2.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/ui/world_view/world_view_fwd.hpp>

class QPainter;

namespace ramen
{
namespace ui
{

/**
\ingroup ui
\brief draws nodes in the node graph view.
*/
class RAMEN_API world_view_hook_t
{
public:

    world_view_hook_t();
    virtual ~world_view_hook_t();

    virtual math::point2f_t input_location( const node_t& n, std::size_t index) const;
    virtual math::point2f_t output_location( const node_t& n, std::size_t index) const;

    void draw_edges( const node_t& n, world_view_t& view) const;
    virtual void draw_node( const node_t& n, world_view_t& view) const;

    // factory
    typedef world_view_hook_t*(*create_hook_fun)();

    static void init();

    static void register_hook( const core::name_t& id, create_hook_fun fun);

    static world_view_hook_t& hook_for_node( const core::name_t& id);
    static world_view_hook_t& hook_for_node( const node_t& n);

private:

    // non-copyable
    world_view_hook_t( const world_view_hook_t&);
    world_view_hook_t& operator=( const world_view_hook_t&);
};

} // ui
} // ramen

#endif
