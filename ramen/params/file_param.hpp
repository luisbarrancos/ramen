// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/static_param.hpp>

#include <QPointer>

#include <ramen/ui/widgets/line_edit_fwd.hpp>

class QPushButton;

namespace ramen
{
class RAMEN_API file_param_t : public static_param_t
{
    Q_OBJECT

  public:
    explicit file_param_t(const std::string& name);

    void set_default_value(const boost::filesystem::path& p);

    void set_value(
        const boost::filesystem::path& p,
        change_reason                  reason = user_edited);

    void set_is_input(bool b) { is_input_ = b; }

    bool empty() const;
    bool file_exists() const;

    // extensions
    void set_extension_list_string(const std::string& str)
    {
        ext_list_string_ = str;
    }

    std::string extension() const;
    void        set_extension(const std::string& ext);

  protected:
    file_param_t(const file_param_t& other);
    void operator=(const file_param_t& other);

  private:
    param_t* do_clone() const override { return new file_param_t(*this); }

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    // paths
    void do_convert_relative_paths(
        const boost::filesystem::path& old_base,
        const boost::filesystem::path& new_base) override;
    void do_make_paths_absolute() override;
    void do_make_paths_relative() override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    void do_update_widgets() override;
    void do_enable_widgets(bool e) override;

    void update_input_text(const boost::filesystem::path& p);
    void update_input_text();

    QWidget* do_create_widgets() override;

    bool is_input_;

    QPointer<ui::line_edit_t> input_;
    QPointer<QPushButton>     button_;

    std::string ext_list_string_;

  private Q_SLOTS:

    void select_pushed();
    void text_changed();
};

}  // namespace ramen
