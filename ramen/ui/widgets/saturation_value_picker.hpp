// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <QWidget>
#include <QImage>

namespace ramen
{
namespace ui
{
class saturation_value_picker_t : public QWidget
{
    Q_OBJECT

  public:
    saturation_value_picker_t(QWidget* parent = 0);

    QSize sizeHint() const override;

    double hue() const { return hue_; }
    double saturation() const { return saturation_; }
    double value() const { return value_; }

  Q_SIGNALS:

    void saturation_value_changed(double sat, double val);

  public Q_SLOTS:

    void set_hue(double h);
    void set_saturation_value(double s, double v);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

  private:
    void pick_sat_val(QMouseEvent* event);

    void update_background();

    QImage background_;
    bool   valid_background_;

    double hue_;
    double saturation_;
    double value_;
};

}  // namespace ui
}  // namespace ramen
