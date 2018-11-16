// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <QComboBox>

#include <string>

namespace ramen
{
namespace ui
{
class ocio_combo_t : public QComboBox
{
    Q_OBJECT

public:
    ocio_combo_t(QWidget* parent = 0);

protected:
    int index_for_string(const std::string& s) const;
};

}  // ui
}  // ramen

