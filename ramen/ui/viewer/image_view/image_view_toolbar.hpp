// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <QWidget>

class QComboBox;
class QToolButton;

#include <ramen/nodes/node_fwd.hpp>

namespace ramen
{
namespace ui
{
namespace viewer
{
class image_viewer_strategy_t;

class image_view_toolbar_t : public QWidget
{
    Q_OBJECT

  public:
    image_view_toolbar_t(image_viewer_strategy_t* image_viewer);

    void update_widgets(node_t* n);

  private:
    image_viewer_strategy_t* image_viewer_;

    QComboBox*   resolution_combo_;
    QComboBox*   proxy_combo_;
    QComboBox*   channels_combo_;
    QToolButton* mblur_btn_;
    QToolButton* show_checks_btn_;
    QToolButton* show_interest_btn_;
    QToolButton* show_overlay_btn_;
};

}  // namespace viewer
}  // namespace ui
}  // namespace ramen
