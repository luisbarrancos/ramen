// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/ui/widgets/time_slider_fwd.hpp>

#include <QWidget>

#include <ramen/ui/widgets/double_spinbox_fwd.hpp>

class QSlider;

namespace ramen
{
namespace ui
{
class time_scale_t : public QWidget
{
    Q_OBJECT

public:
    time_scale_t(QWidget* parent = 0);

    void setRange(int lo, int hi);
    void setMinimum(int m);
    void setMaximum(int m);

    void setValue(int v);

protected:
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void paintEvent(QPaintEvent* event) override;

Q_SIGNALS:

    void valueChanged(double value);

private:
    int round_halfup(float x) const;
    int frame_from_mouse_pos(int x) const;

    int min_value_;
    int max_value_;
    int value_;

    int  last_x_;
    bool drag_;
};

class time_slider_t : public QWidget
{
    Q_OBJECT

public:
    time_slider_t(QWidget* parent = 0);

    void update(int start, int frame, int end);

public Q_SLOTS:

    void set_start_frame(double t);
    void set_end_frame(double t);
    void set_frame(double t);

Q_SIGNALS:

    void start_frame_changed(int t);
    void end_frame_changed(int t);
    void time_changed(int t);

private:
    void block_all_signals(bool b);
    void adjust_frame(int frame);

    double_spinbox_t *start_, *end_, *current_;
    QSlider*          slider_;
    time_scale_t*     scale_;
};

}  // namespace
}  // namespace

