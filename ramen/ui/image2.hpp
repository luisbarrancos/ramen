// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_IMAGE2_HPP
#define	RAMEN_UI_IMAGE2_HPP

#include<ramen/config.hpp>

#include<ramen/core/memory.hpp>

#include<ramen/math/box2.hpp>

#include<ramen/image/buffer.hpp>

#include<ramen/GL/gl.hpp>

namespace ramen
{
namespace ui
{

class image2_t
{
public:

    image2_t();

    image2_t( image::buffer_t pixels,
              const math::box2i_t& display_window,
              const math::box2i_t& data_window);

    ~image2_t();

    void reset();
    void reset( image::buffer_t pixels,
                const math::box2i_t& display_window,
                const math::box2i_t& data_window);

    math::box2i_t display_window() const;
    math::box2i_t data_window() const;

    // draw
    void draw() const;

private:

    struct impl;
    core::auto_ptr_t<impl> pimpl_;
};

} // ui
} // ramen

#endif
