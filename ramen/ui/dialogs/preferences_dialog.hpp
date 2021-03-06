// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <QDialog>

#include "ui_preferences.h"

namespace ramen
{
namespace ui
{
class preferences_dialog_t : public QDialog
{
    Q_OBJECT

  public:
    static preferences_dialog_t& instance();

    void exec_dialog();

  private:
    preferences_dialog_t();

    Ui::preferences ui_;
};

}  // namespace ui
}  // namespace ramen
