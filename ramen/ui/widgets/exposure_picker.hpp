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
class exposure_picker_t : public QWidget
{
    Q_OBJECT

public:
    exposure_picker_t(QWidget* parent = 0);

    QSize sizeHint() const override;

    static double max_exposure();
    double        exposure() const;

Q_SIGNALS:

    void exposure_changed(double e);

public Q_SLOTS:

    void set_exposure(double e);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

private:
    double pick_exposure(int x) const;

    void update_background();

    QImage background_;
    bool   valid_background_;
    double exposure_;
};

}  // ui
}  // ramen

