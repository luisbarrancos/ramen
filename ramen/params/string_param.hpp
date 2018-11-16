// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/static_param.hpp>

#include <QPointer>

#include <ramen/ui/widgets/line_edit_fwd.hpp>
#include <ramen/ui/widgets/text_edit_fwd.hpp>

namespace ramen
{
class string_param_t : public static_param_t
{
    Q_OBJECT

  public:
    explicit string_param_t(const std::string& name);
    string_param_t(const std::string& name, bool read_only);

    bool read_only() const { return read_only_; }

    bool multiline() const { return multiline_; }
    void set_multiline(bool b) { multiline_ = b; }

    void set_default_value(const std::string& x);

    void set_value(const std::string& x, change_reason reason = user_edited);

  protected:
    string_param_t(const string_param_t& other);
    void operator=(const string_param_t& other);

  private:
    param_t* do_clone() const override { return new string_param_t(*this); }

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    QWidget* do_create_widgets() override;
    void     do_update_widgets() override;
    void     do_enable_widgets(bool e) override;

    QPointer<ui::line_edit_t> input_;
    QPointer<ui::text_edit_t> multi_input_;
    bool                      read_only_;
    bool                      multiline_;

  private Q_SLOTS:

    void text_changed();
};

}  // namespace ramen
