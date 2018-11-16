// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/params/static_param.hpp>

#include <string>
#include <vector>

#include <QPointer>

#include <ramen/ui/widgets/ocio_colorspace_combo_fwd.hpp>

namespace ramen
{
class RAMEN_API ocio_colorspace_param_t : public static_param_t
{
    Q_OBJECT

public:
    explicit ocio_colorspace_param_t(const std::string& name);

    void set_default_value(const std::string& cs);
    void set_value(const std::string& cs, change_reason reason = user_edited);

protected:
    ocio_colorspace_param_t(const ocio_colorspace_param_t& other);
    void operator=(const ocio_colorspace_param_t& other);

private:
    param_t* do_clone() const override { return new ocio_colorspace_param_t(*this); }

    void do_add_to_hash(hash::generator_t& hash_gen) const override;

    void do_read(const serialization::yaml_node_t& node) override;
    void do_write(serialization::yaml_oarchive_t& out) const override;

    QWidget* do_create_widgets() override;
    void     do_update_widgets() override;
    void     do_enable_widgets(bool e) override;

    std::string default_colorspace() const;

    QPointer<ui::ocio_colorspace_combo_t> menu_;

private Q_SLOTS:

    void colorspace_picked(const std::string& cs);
};

}  // namespace

