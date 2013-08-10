// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_OCIO_COLORSPACE_PARAM_HPP
#define	RAMEN_OCIO_COLORSPACE_PARAM_HPP

#include<ramen/params/static_param.hpp>

#include<QPointer>

#include<ramen/qwidgets/ocio_colorspace_combo.hpp>

namespace ramen
{

class RAMEN_API ocio_colorspace_param_t : public static_param_t
{
    Q_OBJECT

public:

    ocio_colorspace_param_t();

    void set_default_value( const core::string8_t& cs);
    void set_value( const core::string8_t& cs, change_reason reason = user_edited);

protected:

    ocio_colorspace_param_t( const ocio_colorspace_param_t& other);
    void operator=( const ocio_colorspace_param_t& other);

private:

    virtual param_t *do_clone() const { return new ocio_colorspace_param_t( *this);}

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    //virtual void do_read( const serialization::yaml_node_t& node);
    //virtual void do_write( serialization::yaml_oarchive_t& out) const;

    virtual QWidget *do_create_widgets();
    virtual void do_update_widgets();
    virtual void do_enable_widgets( bool e);

    core::string8_t default_colorspace() const;

    QPointer<qwidgets::ocio_colorspace_combo_t> menu_;

private Q_SLOTS:

    void colorspace_picked( const QString& cs);
};

} // ramen

#endif
