// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_VIEWER_TABS_CONTAINER_HPP
#define	RAMEN_UI_VIEWER_VIEWER_TABS_CONTAINER_HPP

#include<ramen/config.hpp>

#include<QTabWidget>

namespace ramen
{
namespace ui
{

class viewer_tabs_container_t : public QTabWidget
{
    Q_OBJECT

public:

    viewer_tabs_container_t( QWidget *parent = 0);
};

} // ui
} // ramen

#endif
