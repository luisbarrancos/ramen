// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/static_param.hpp>

#include <QPointer>
#include <QCheckBox>

namespace ramen
{
class RAMEN_API bool_param_t : public static_param_t
{
    Q_OBJECT

public:
    explicit bool_param_t(const std::string& name);

    void set_default_value(bool x);

    void set_value(bool x, change_reason reason = user_edited);

protected:
    bool_param_t(const bool_param_t& other);
    void operator=(const bool_param_t& other);

private:
    param_t* do_clone() const override;

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    void do_update_widgets() override;
    void do_enable_widgets(bool e) override;

    QWidget* do_create_widgets() override;

    QPointer<QCheckBox> button_;

private Q_SLOTS:

    void button_checked(int state);
};

}  // namespace

