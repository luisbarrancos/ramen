// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_HPP
#define	RAMEN_UI_VIEWER_HPP

#include<ramen/ui/viewer/viewer_fwd.hpp>

#include<string>
#include<vector>
#include<map>

#include<QObject>

#include<map>

#include<boost/function.hpp>

#include<ramen/nodes/node_fwd.hpp>

class QWidget;
class QFrame;
class QPushButton;
class QComboBox;
class QDoubleSpinBox;
class QToolButton;
class QLabel;

namespace ramen
{
namespace ui
{

class viewer_t : public QObject
{
    Q_OBJECT

public:

    viewer_t();
    ~viewer_t();

    QWidget *widget() { return window_;}

    void set_active_node( node_t *n);
    void set_context_node( node_t *n);

    void frame_changed();

    void begin_interaction();
    void end_interaction();

private:

    void node_released( node_t *n);

    QWidget *window_;
};

} // ui
} // ramen

#endif
