// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_STRING_PARAM_HPP
#define	RAMEN_STRING_PARAM_HPP

#include<ramen/params/static_param.hpp>

#include<QPointer>

class QLineEdit;
class QPlainTextEdit;

namespace ramen
{

class string_param_t : public static_param_t
{
    Q_OBJECT

public:

    explicit string_param_t( bool read_only);

    bool read_only() const { return read_only_;}

    bool multiline() const		{ return multiline_;}
    void set_multiline( bool b)	{ multiline_ = b;}

    void set_default_value( core::string8_t x);

    void set_value( core::string8_t x, change_reason reason = user_edited);

protected:

    string_param_t( const string_param_t& other);
    void operator=( const string_param_t& other);

private:

    virtual param_t *do_clone() const { return new string_param_t( *this);}

    virtual void do_add_to_hash( hash::generator_t& hash_gen) const;

    //virtual void do_read( const serialization::yaml_node_t& node);
    //virtual void do_write( serialization::yaml_oarchive_t& out) const;

    virtual QWidget *do_create_widgets();
    virtual void do_update_widgets();
    virtual void do_enable_widgets( bool e);

    QPointer<QLineEdit> input_;
    QPointer<QPlainTextEdit> multi_input_;
    bool read_only_;
    bool multiline_;

private Q_SLOTS:

    void text_changed();
};

} // ramen

#endif
