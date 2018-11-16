// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/param.hpp>

#include <QPointer>

class QPushButton;

namespace ramen
{
class RAMEN_API button_param_t : public param_t
{
    Q_OBJECT

public:
    explicit button_param_t(const std::string& name);

protected:
    button_param_t(const button_param_t& other);
    void operator=(const button_param_t& other);

private:
    param_t* do_clone() const override { return new button_param_t(*this); }

    void do_enable_widgets(bool e) override;

    QWidget* do_create_widgets() override;

    QPointer<QPushButton> button_;

private Q_SLOTS:

    void button_pressed();
};

}  // namespace

