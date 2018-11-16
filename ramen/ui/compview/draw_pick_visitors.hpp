// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/node_visitor.hpp>

#include <OpenEXR/ImathBox.h>

#include <ramen/ui/compview/pick_result.hpp>

class QPainter;

namespace ramen
{
namespace ui
{
class composition_view_t;

struct input_location_node_visitor : public generic_node_visitor
{
    input_location_node_visitor(int i = 0);
    void visit(node_t* n) override;

    int        index;
    Imath::V2f loc;
};

struct output_location_node_visitor : public generic_node_visitor
{
    output_location_node_visitor(int i = 0);
    void visit(node_t* n) override;

    int        index;
    Imath::V2f loc;
};

struct size_node_visitor : public generic_node_visitor
{
    size_node_visitor();
    void visit(node_t* n) override;

    Imath::V2i size;
};

struct draw_node_visitor : public generic_node_visitor
{
    draw_node_visitor(QPainter& painter);
    void visit(node_t* n) override;

private:
    QPainter& painter_;
};

struct draw_edges_visitor : public generic_node_visitor
{
    draw_edges_visitor(const composition_view_t& view, QPainter& painter);
    void visit(node_t* n) override;

private:
    const composition_view_t& view_;
    QPainter&                 painter_;
};

struct pick_node_visitor : public generic_node_visitor
{
    pick_node_visitor(const composition_view_t& view, const Imath::V2f& p, pick_result_t& result);
    void visit(node_t* n) override;

private:
    const composition_view_t& view_;
    Imath::V2f                p_;
    pick_result_t&            result_;
};

struct box_pick_node_visitor : public generic_node_visitor
{
    box_pick_node_visitor(const Imath::Box2f& box);
    void visit(node_t* n) override;

    bool result;

private:
    Imath::Box2f box_;
};

struct pick_edge_visitor : public generic_node_visitor
{
    pick_edge_visitor(const composition_view_t& view, const Imath::V2f& p);
    void visit(node_t* n) override;

    node_t *src, *dst;
    int     port;

private:
    Imath::V2f                p_;
    const composition_view_t& view_;
};

}  // ui
}  // ramen

