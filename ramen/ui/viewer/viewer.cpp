// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/viewer/viewer.hpp>

#include<QWidget>

#include<ramen/nodes/composition_node.hpp>

namespace ramen
{
namespace ui
{

viewer_t::viewer_t() : QObject()
{
    // normal widgets
    window_ = new QWidget();
    window_->setWindowTitle( "Viewer");

    composition_node_t::node_deleted.connect( boost::bind( &viewer_t::node_released, this, _1));
}

viewer_t::~viewer_t() {}

void viewer_t::set_active_node( node_t *n) {}
void viewer_t::set_context_node( node_t *n) {}

void viewer_t::node_released( node_t *n) {}

void viewer_t::frame_changed() {}

void viewer_t::begin_interaction() {}
void viewer_t::end_interaction() {}

} // ui
} // ramen
