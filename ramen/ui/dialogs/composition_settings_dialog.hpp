// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/config.hpp>

#include <QDialog>

#include "ui_composition_settings.h"

namespace ramen
{
namespace ui
{
class image_format_widget_t;

class composition_settings_dialog_t : public QDialog
{
    Q_OBJECT

  public:
    static composition_settings_dialog_t& instance();

    void exec_dialog();

  private:
    composition_settings_dialog_t();

    Ui::composition_settings ui_;
};

}  // namespace ui
}  // namespace ramen
