// Copyright (c) 2011 Esteban Tovagliari

#pragma once

#include <QPlainTextEdit>

namespace ramen
{
namespace ui
{
class text_edit_t : public QPlainTextEdit
{
    Q_OBJECT

  public:
    text_edit_t(QWidget* parent = 0);

  protected:
    void focusOutEvent(QFocusEvent* e) override;

  Q_SIGNALS:

    void textHasChanged();
};

}  // namespace ui
}  // namespace ramen
