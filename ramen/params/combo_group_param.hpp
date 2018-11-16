// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#ifndef Q_MOC_RUN
#include <ramen/params/composite_param.hpp>
#endif

#include <QPointer>

class QComboBox;
class QStackedWidget;

namespace ramen
{
class RAMEN_API combo_group_param_t : public composite_param_t
{
    Q_OBJECT

public:
    explicit combo_group_param_t(const std::string& name);

    void set_default_value(int x);

    void set_value(int x, change_reason reason = user_edited);

protected:
    combo_group_param_t(const combo_group_param_t& other);
    void operator=(const combo_group_param_t& other);

private:
    param_t* do_clone() const override { return new combo_group_param_t(*this); }

    void do_update_widgets() override;
    void do_enable_widgets(bool e) override;

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    virtual std::auto_ptr<undo::command_t> do_create_command();

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    QWidget* do_create_widgets() override;

    QPointer<QComboBox>      menu_;
    QPointer<QStackedWidget> stack_;

private Q_SLOTS:

    void item_picked(int index);
};

}  // namespace

