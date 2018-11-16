// Copyright (c) 2011 Esteban Tovagliari

#pragma once

#include <ramen/ui/widgets/line_edit_fwd.hpp>

#include <QLineEdit>

namespace ramen
{
namespace ui
{
class line_edit_t : public QLineEdit
{
    Q_OBJECT

public:
    line_edit_t(QWidget* parent = 0);
    line_edit_t(const QString& contents, QWidget* parent = 0);

protected:
    void contextMenuEvent(QContextMenuEvent* event) override;
};

}  // ui
}  // ramen

