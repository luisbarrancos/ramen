// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_COMPOSITION_VIEW_LAYOUT_HPP
#define	RAMEN_UI_COMPOSITION_VIEW_LAYOUT_HPP

#include<boost/optional.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/math/box2.hpp>

namespace ramen
{
namespace ui
{

class composition_view_layout_t
{
public:

    composition_view_layout_t();

    void set_world( const math::box2f_t& w);

    void set_interest_point( const math::point2f_t& p);

    void place_node( node_t *n) const;
    void place_node_near_node( node_t *n, node_t *other) const;

private:

    math::box2f_t get_node_bbox( node_t *n) const;
    bool box_intersects_any_node( const math::box2f_t& box) const;

    void do_place_node( node_t *n, const math::point2f_t& p) const;

    math::box2f_t world_;
    boost::optional<math::point2f_t> interest_point_;
};

} // ui
} // ramen

#endif
