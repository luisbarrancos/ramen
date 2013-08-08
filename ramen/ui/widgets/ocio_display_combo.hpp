// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_QR_OCIO_DISPLAY_COMBO_HPP
#define RAMEN_QR_OCIO_DISPLAY_COMBO_HPP

#include<ramen/ui/widgets/ocio_combo.hpp>

namespace ramen
{
namespace ui
{

class ocio_display_combo_t : public ocio_combo_t
{
    Q_OBJECT

public:

    ocio_display_combo_t( QWidget *parent = 0);

    const core::string8_t& get_current_display() const { return current_display_;}

public Q_SLOTS:

    void set_display( const core::string8_t& s);

Q_SIGNALS:

    void display_changed( const core::string8_t&);

private Q_SLOTS:

    void combo_index_changed( int indx);

private:

    void set_default();

    core::string8_t current_display_;
};

} // ui
} // ramen

#endif
