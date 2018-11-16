// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <QDialog>

#include "ui_smooth_anim_curves.h"

namespace ramen
{
namespace ui
{
class smooth_anim_curves_dialog_t : public QDialog
{
    Q_OBJECT

  public:
    static smooth_anim_curves_dialog_t& instance();

    bool exec(bool high_pass, float& filter_size, bool& resample);

  private:
    smooth_anim_curves_dialog_t();

    Ui::smooth_anim_curves ui_;
};

}  // namespace ui
}  // namespace ramen
