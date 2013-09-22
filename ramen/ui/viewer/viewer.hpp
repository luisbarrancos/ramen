// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_VIEWER_HPP
#define	RAMEN_UI_VIEWER_VIEWER_HPP

#include<ramen/ui/viewer/viewer_fwd.hpp>

#include<QWidget>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{
namespace ui
{

class viewer_t : public QWidget
{
    Q_OBJECT

public:

    explicit viewer_t( nodes::viewer_node_t *v);
    ~viewer_t();

private:

    nodes::viewer_node_t *viewer_node_;
};

} // ui
} // ramen

#endif
