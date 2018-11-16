// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/static_param.hpp>

#include <ramen/image/format.hpp>

#include <QPointer>

#include <ramen/ui/widgets/double_spinbox_fwd.hpp>

class QComboBox;

namespace ramen
{
class RAMEN_API image_format_param_t : public static_param_t
{
    Q_OBJECT

  public:
    explicit image_format_param_t(const std::string& name);

    void set_value(const image::format_t& format, change_reason = user_edited);

  protected:
    image_format_param_t(const image_format_param_t& other);
    void operator=(const image_format_param_t& other);

  private:
    void     do_init() override;
    param_t* do_clone() const override;

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    QWidget* do_create_widgets() override;
    void     do_update_widgets() override;
    void     do_enable_widgets(bool e) override;

    QPointer<ui::double_spinbox_t> width_input_, height_input_, aspect_input_;
    QPointer<QComboBox>            menu_;

  private Q_SLOTS:

    void preset_picked(int v);
    void set_new_format(double unused = 0);
};

}  // namespace ramen
