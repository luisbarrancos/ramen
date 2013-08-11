// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_HPP
#define	RAMEN_UI_VIEWER_HPP

#include<ramen/ui/viewer_fwd.hpp>

#include<QWidget>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/ui/image_view_fwd.hpp>

namespace ramen
{
namespace ui
{

class viewer_t : public QWidget
{
    Q_OBJECT

public:

    viewer_t();
    ~viewer_t();

    void set_active_node( node_t *n);
    void set_context_node( node_t *n);

    void frame_changed();

    void begin_interaction();
    void end_interaction();

private:

    void node_released( node_t *n);

    image_view_t *img_view_;
};

} // ui
} // ramen

#endif
