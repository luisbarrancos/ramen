// Copyright (c) 2010 Esteban Tovagliari.
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/preferences.hpp>

#include<stdlib.h>
#include<stdio.h>

#include<boost/filesystem/fstream.hpp>
#include<boost/filesystem/operations.hpp>
#include<boost/filesystem/convenience.hpp>

#include<ramen/app/application.hpp>

namespace ramen
{

preferences_t::preferences_t()
{
    set_defaults();
    load();
}

int preferences_t::max_image_width() const	{ return 16 * 1024;} // 16K
int preferences_t::max_image_height() const	{ return 16 * 1024;} // 16K

void preferences_t::set_defaults()
{
    max_image_memory_ = 30; // % of ram

    frame_rate_ = 25; // PAL by default.

    pick_distance_ = 5;
}

void preferences_t::load()
{
}

void preferences_t::save()
{
}

} // ramen
