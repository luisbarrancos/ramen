// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <QWidget>
#include <QImage>

#include <ramen/ui/widgets/color.hpp>

namespace ramen
{
namespace ui
{
class color_swatch_t : public QWidget
{
    Q_OBJECT

  public:
    color_swatch_t(QWidget* parent, const color_t& c);

    QSize sizeHint() const override;
    int   heightForWidth(int w) const override { return w; }

  public Q_SLOTS:

    void set_color(const ramen::ui::color_t& c);

  protected:
    void paintEvent(QPaintEvent* event) override;
    void resizeEvent(QResizeEvent* event) override;

  private:
    void update_background();

    inline double clamp(double x) const
    {
        if (x < 0)
            return 0;
        if (x > 1)
            return 1;
        return x;
    }

    QImage  background_;
    bool    valid_background_;
    color_t color_;
};

}  // namespace ui
}  // namespace ramen
