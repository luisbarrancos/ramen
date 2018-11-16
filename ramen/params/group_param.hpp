// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#ifndef Q_MOC_RUN
#include <ramen/params/composite_param.hpp>
#endif

#include <QPointer>

class QGroupBox;

namespace ramen
{
class RAMEN_API group_param_t : public composite_param_t
{
    Q_OBJECT

  public:
    explicit group_param_t(const std::string& name);

    bool can_collapse() const { return can_collapse_; }
    void set_can_collapse(bool b) { can_collapse_ = b; }

  protected:
    group_param_t(const group_param_t& other);
    void operator=(const group_param_t& other);

  private:
    param_t* do_clone() const override { return new group_param_t(*this); }

    QWidget* do_create_widgets() override;
    void     do_enable_widgets(bool e) override;

    QPointer<QGroupBox> group_;
    bool                can_collapse_;
};

}  // namespace ramen
