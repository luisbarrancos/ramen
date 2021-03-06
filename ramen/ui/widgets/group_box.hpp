// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <QGroupBox>

namespace ramen
{
namespace ui
{
class group_box_t : public QGroupBox
{
    Q_OBJECT

  public:
    group_box_t(QWidget* parent = 0);

  protected:
    void mousePressEvent(QMouseEvent* e) override;
    void mouseReleaseEvent(QMouseEvent* e) override;

  private:
    void collapse(bool b);

    bool collapsed_;
    bool clicked_;
};

}  // namespace ui
}  // namespace ramen
