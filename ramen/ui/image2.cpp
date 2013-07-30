// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/image2.hpp>

#include<iostream>

#include<boost/ptr_container/ptr_vector.hpp>
#include<boost/bind.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace ui
{

struct image2_t::impl
{
    struct tile_t
    {
        enum { tile_size = 256};

        tile_t()
        {
            texture_id_ = gl_gen_texture();
            gl_bind_texture( GL_TEXTURE_2D, texture_id_);
            gl_tex_parameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            gl_tex_parameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            gl_tex_parameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            gl_tex_parameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            //gl_tex_image2d( GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        }

        void draw() const
        {
            gl_bind_texture( GL_TEXTURE_2D, texture_id_);
            // draw quad here...
        }

        GLuint texture_id_;
        math::point2i_t pos;
        math::vector2i_t size;
    };

    impl() {}

    void reset()
    {
        display_window_ = math::box2i_t();
        data_window_ = math::box2i_t();
        pixels_.clear();
        tiles_.clear();
    }

    void reset( image::buffer_t pixels,
                const math::box2i_t& display_window,
                const math::box2i_t& data_window)
    {
        pixels_ = pixels;
        display_window_ = display_window;
        data_window_ = data_window;

        int xtiles = std::ceil( ( double) ( data_window.size().x + 1) / tile_t::tile_size);
        int ytiles = std::ceil( ( double) ( data_window.size().y + 1) / tile_t::tile_size);
        int num_tiles = xtiles * ytiles;

        if( tiles_.size() > num_tiles)
            tiles_.resize( num_tiles);

        for( int j = 0; j < ytiles; ++j)
        {
            for( int i = 0; i < xtiles; ++i)
            {
                if( i * j < tiles_.size())
                {
                    // reuse an existing tile
                }
                else
                {
                    // create a new tile
                }
            }
        }
    }

    void draw() const
    {
        boost::range::for_each( tiles_, boost::bind( &tile_t::draw, _1));
    }

    math::box2i_t display_window_, data_window_;
    image::buffer_t pixels_;
    boost::ptr_vector<tile_t> tiles_;
};

image2_t::image2_t()
{
    pimpl_.reset( new impl());
}

image2_t::image2_t( image::buffer_t pixels,
                    const math::box2i_t& display_window,
                    const math::box2i_t& data_window)
{
    pimpl_.reset( new impl());
    reset( pixels, display_window, data_window);
}

image2_t::~image2_t()
{
    // Do not remove.
}

void image2_t::reset()
{
    RAMEN_ASSERT( pimpl_.get());

    pimpl_->reset();
}

void image2_t::reset( image::buffer_t pixels,
                      const math::box2i_t& display_window,
                      const math::box2i_t& data_window)
{
    RAMEN_ASSERT( pimpl_.get());

    pimpl_->reset( pixels, display_window, data_window);
}

math::box2i_t image2_t::display_window() const
{
    RAMEN_ASSERT( pimpl_.get());

    return pimpl_->display_window_;
}

math::box2i_t image2_t::data_window() const
{
    RAMEN_ASSERT( pimpl_.get());

    return pimpl_->data_window_;
}

void image2_t::draw() const
{
    RAMEN_ASSERT( pimpl_.get());

    pimpl_->draw();
}

} // ui
} // ramen
