// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/ui/widgets/double_spinbox_fwd.hpp>
#include <ramen/ui/widgets/spinbox.hpp>

#include <string>

namespace ramen
{
namespace ui
{
class double_spinbox_t : public spinbox_t
{
    Q_OBJECT

  public:
    double_spinbox_t(QWidget* parent = 0);

    void setSuffix(const QString& s);

    double value() const;
    void   restorePreviousValue();

  public Q_SLOTS:

    bool setValue(double v);

  Q_SIGNALS:

    void valueChanged(double value);
    void spinBoxPressed();
    void spinBoxDragged(double value);
    void spinBoxReleased();

  protected:
    void keyPressEvent(QKeyEvent* event) override;
    void keyReleaseEvent(QKeyEvent* event) override;

    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;

    void contextMenuEvent(QContextMenuEvent* event) override;

  private Q_SLOTS:

    void textChanged();

  private:
    void stepBy(int steps);

    double value_;
    double previous_value_;
};

}  // namespace ui
}  // namespace ramen
