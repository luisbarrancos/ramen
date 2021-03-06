// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/static_param.hpp>

#include <string>
#include <vector>

#ifndef Q_MOC_RUN
#include <boost/assign/list_of.hpp>
#endif

#include <QPointer>

class QComboBox;

namespace ramen
{
class RAMEN_API popup_param_t : public static_param_t
{
    Q_OBJECT

  public:
    explicit popup_param_t(const std::string& name);

    void set_default_value(int x);

    void set_value(int x, change_reason reason = user_edited);

    void add_menu_item(const std::string& item);

    const std::vector<std::string>& menu_items() const { return menu_items_; }
    std::vector<std::string>&       menu_items() { return menu_items_; }

  protected:
    popup_param_t(const popup_param_t& other);
    void operator=(const popup_param_t& other);

  private:
    param_t* do_clone() const override { return new popup_param_t(*this); }

    void do_evaluate(float time) override {}

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    int find_index_for_string(const std::string& s) const;

    std::vector<std::string> menu_items_;

    QWidget* do_create_widgets() override;
    void     do_update_widgets() override;
    void     do_enable_widgets(bool e) override;

    QPointer<QComboBox> menu_;

  private Q_SLOTS:

    void item_picked(int index);
};

}  // namespace ramen
