// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/params/param.hpp>

namespace ramen
{
class RAMEN_API separator_param_t : public param_t
{
    Q_OBJECT

  public:
    separator_param_t();
    explicit separator_param_t(const std::string& name);

  protected:
    separator_param_t(const separator_param_t& other);
    void operator=(const separator_param_t& other);

  private:
    param_t* do_clone() const override { return new separator_param_t(*this); }

    QWidget* do_create_widgets() override;
};

}  // namespace ramen
