// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_IMAGE_HPP
#define	RAMEN_UI_VIEWER_IMAGE_HPP

#include<ramen/core/memory.hpp>

#include<boost/optional.hpp>

#include<ramen/math/box2.hpp>
#include<ramen/color/rgba_color.hpp>

#include<ramen/image/buffer.hpp>

#include<ramen/GL/gl.hpp>

namespace ramen
{
namespace ui
{
namespace viewer
{

class image_t
{
public:

    image_t( GLenum texture_unit = GL_TEXTURE0);
    ~image_t();

    bool valid() const;

    void reset();
    void reset( image::buffer_t pixels,
                const math::box2i_t& display_window,
                const math::box2i_t& data_window);

    math::box2i_t display_window() const;
    math::box2i_t data_window() const;

    // draw
    void draw_background() const;
    void draw() const;

    // overlay draw
    void frame_display_window() const;
    void frame_data_window() const;

    // get colors
    boost::optional<color::rgba_colorf_t> color_at( const math::point2i_t& p) const;

private:

    void create_impl( const image::buffer_t& pixels,
                      const math::box2i_t& display_window,
                      const math::box2i_t& data_window);

    void frame_rect( const math::box2i_t& b) const;
    void gl_vertices_for_box( const math::box2i_t& b) const;

    GLenum texture_unit_;

    struct impl;
    core::auto_ptr_t<impl> pimpl_;
};

} // viewer
} // ui
} // ramen

#endif
