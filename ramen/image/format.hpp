// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_FORMAT_HPP
#define RAMEN_IMAGE_FORMAT_HPP

#include<vector>
#include<utility>

#include<ramen/core/string8.hpp>

#include<ramen/math/box2.hpp>

namespace ramen
{
namespace image
{

struct format_t
{
    format_t();
    format_t( int w, int h, float asp = 1.0f);

    math::box2i_t area() const;

    // presets
    typedef std::pair<core::string8_t, format_t> preset_type;

    static const std::vector<preset_type>& presets();

    int preset_index() const;
    void set_from_preset( int index);

    void set_default();

    // operators
    bool operator==( const format_t& other) const;
    bool operator!=( const format_t& other) const;

    static void init_presets();
    static void add_preset( const preset_type& p);

    // data
    int width, height;
    float aspect;

private:

    static std::vector<preset_type> presets_;
};

std::ostream& operator<<( std::ostream& os, const format_t& f);

} // image
} // ramen

#endif
