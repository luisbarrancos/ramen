// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_QR_OCIO_VIEW_COMBO_HPP
#define RAMEN_QR_OCIO_VIEW_COMBO_HPP

#include<ramen/ui/widgets/ocio_combo.hpp>

namespace ramen
{
namespace ui
{

class ocio_view_combo_t : public ocio_combo_t
{
    Q_OBJECT

public:

    ocio_view_combo_t( QWidget *parent = 0);

    const core::string8_t& get_current_view() const { return current_view_;}

public Q_SLOTS:

    void set_view( const core::string8_t& s);

    void update_views( const core::string8_t& display);

Q_SIGNALS:

    void view_changed( const core::string8_t&);

private Q_SLOTS:

    void combo_index_changed( int indx);

private:

    core::string8_t current_view_;
};

} // ui
} // ramen

#endif
