// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/composite_param.hpp>

#include <QPointer>

class QTabWidget;

namespace ramen
{
class tab_group_param_t : public composite_param_t
{
    Q_OBJECT

  public:
    tab_group_param_t();
    explicit tab_group_param_t(const std::string& name);

  protected:
    tab_group_param_t(const tab_group_param_t& other);
    void operator=(const tab_group_param_t& other);

  private:
    param_t* do_clone() const override { return new tab_group_param_t(*this); }

    QWidget* do_create_widgets() override;
    void     do_enable_widgets(bool e) override;

    void do_create_tracks(anim::track_t* parent) override;

    QPointer<QTabWidget> tab_;
};

}  // namespace ramen
