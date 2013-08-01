// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/viewer/image_view/image.hpp>

#include<algorithm>
#include<iostream>

#include<boost/ptr_container/ptr_vector.hpp>
#include<boost/bind.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace ui
{
namespace viewer
{

struct image_t::impl
{
    impl( const image::buffer_t& pixels,
          const math::box2i_t& display_window,
          const math::box2i_t& data_window,
          GLenum texture_unit = GL_TEXTURE0)
    {
        RAMEN_ASSERT( !data_window.is_empty());
        RAMEN_ASSERT( !display_window.is_empty());

        texture_unit_ = texture_unit;
        data_window_ = data_window;
        display_window_ = display_window;

        pixels_ = pixels;

        int xtiles = std::ceil( ( double) ( data_window.size().x + 1) / tile_size());
        int ytiles = std::ceil( ( double) ( data_window.size().y + 1) / tile_size());

        for( int j = 0; j < ytiles; ++j)
        {
            for( int i = 0; i < xtiles; ++i)
            {
                math::box2i_t area( area_for_tile( i, j));
                tile_t *tile = new tile_t( pixels_, area);
                tiles_.push_back( tile);
            }
        }

        gl_finish();
        gl_bind_texture( GL_TEXTURE_2D, 0);
    }

    int width() const
    {
        if( !data_window().is_empty())
            return data_window().size().x + 1;

        return 0;
    }

    int height() const
    {
        if( !data_window().is_empty())
            return data_window().size().y + 1;

        return 0;
    }

    const math::box2i_t& display_window() const	{ return display_window_;}
    const math::box2i_t& data_window() const    { return data_window_;}

    bool update_pixels( const image::buffer_t& pixels,
                        const math::box2i_t& display_window,
                        const math::box2i_t& data_window)
    {
        //if( pixels.width() == width() && pixels.height() == height())
        if( data_window.size().x + 1 == width() && data_window.size().y + 1 == height())
        {
            pixels_ = pixels;
            data_window_ = data_window;
            display_window_ = display_window;

            int xtiles = std::ceil( ( double) ( data_window_.size().x + 1) / tile_size());
            int ytiles = std::ceil( ( double) ( data_window_.size().y + 1) / tile_size());
            RAMEN_ASSERT( tiles_.size() == ( xtiles * ytiles));

            std::size_t rowsize = tile_t::rowbytes( pixels_);

            int index = 0;
            for( int j = 0; j < ytiles; ++j)
            {
                for( int i = 0; i < xtiles; ++i)
                {
                    math::box2i_t area( area_for_tile( i, j));
                    char *ptr = tile_t::pixel_ptr( pixels_, area.min.x, area.max.y);
                    tiles_[index].update_texture( area, ptr, rowsize);
                    ++index;
                }
            }

            gl_finish();
            gl_bind_texture( GL_TEXTURE_2D, 0);
            return true;
        }

        return false;
    }

    void draw() const
    {
        gl_active_texture( texture_unit_);
        gl_tex_envf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        boost::range::for_each( tiles_, boost::bind( &tile_t::draw, _1));
    }

    boost::optional<color::rgba_colorf_t> color_at( const math::point2i_t& p) const
    {
        if( p.x < data_window().min.x || p.x > data_window().max.x)
            return boost::optional<color::rgba_colorf_t>();

        if( p.y < data_window().min.y || p.y > data_window().max.y)
            return boost::optional<color::rgba_colorf_t>();

        image::pixel_t px( pixels_.const_rgba_view()( p.x - data_window().min.x,
                                                      p.y - data_window().min.y));
        return color::rgba_colorf_t( boost::gil::get_color( px, boost::gil::red_t()),
                                     boost::gil::get_color( px, boost::gil::green_t()),
                                     boost::gil::get_color( px, boost::gil::blue_t()),
                                     boost::gil::get_color( px, boost::gil::alpha_t()));
    }

private:

    static int tile_size() { return 1024;}

    math::box2i_t area_for_tile( int x, int y) const
    {
        math::box2i_t area;
        area.min.x = ( x * tile_size()) + data_window().min.x;
        area.min.y = ( y * tile_size()) + data_window().min.y;
        area.max.x = std::min( area.min.x + tile_size() - 1, data_window().max.x);
        area.max.y = std::min( area.min.y + tile_size() - 1, data_window().max.y);
        return area;
    }

    struct tile_t
    {
    public:

        tile_t( const image::buffer_t& pixels, const math::box2i_t& area)  : texture_id_( 0)
        {
            RAMEN_ASSERT( !area.is_empty());

            area_ = area;
            alloc_tile( area_.size().x + 1, area_.size().y + 1);
            char *ptr = pixel_ptr( pixels, area_.min.x, area_.max.y);
            std::size_t rowsize = rowbytes( pixels);
            update_texture( area_, ptr, rowsize);
        }

        ~tile_t()
        {
            gl_delete_texture( &texture_id_);
        }

        void update_texture( const math::box2i_t& area, char *ptr, std::size_t rowsize)
        {
            RAMEN_ASSERT( texture_id_ != 0);
            RAMEN_ASSERT( area_.size() == area.size());

            area_ = area;
            char *p = ptr;
            int width = area_.size().x + 1;
            int j = 0;

            gl_bind_texture( GL_TEXTURE_2D, texture_id_);
            gl_pixel_storei(GL_UNPACK_ALIGNMENT, 1);
            gl_pixel_storei( GL_UNPACK_ROW_LENGTH, rowsize / sizeof( image::pixel_t));

            for( int y = area_.max.y; y >= area_.min.y; --y)
            {
                gl_tex_subimage2d( GL_TEXTURE_2D, 0, 0, j, width, 1, GL_RGBA, GL_FLOAT, p);
                p -= rowsize;
                ++j;
            }
        }

        void draw() const
        {
            gl_bind_texture( GL_TEXTURE_2D, texture_id_);
            gl_textured_quad( area_.min.x, area_.min.y, area_.max.x + 1, area_.max.y + 1);
        }

        static char *pixel_ptr( const image::buffer_t& pixels, int x, int y)
        {
            RAMEN_ASSERT( y >= pixels.bounds().min.y && y <= pixels.bounds().max.y);
            RAMEN_ASSERT( x >= pixels.bounds().min.x && x <= pixels.bounds().max.x);
            return ( char *) &( pixels.const_rgba_view()( x - pixels.bounds().min.x,
                                                          y - pixels.bounds().min.y)[0]);
        }

        static std::size_t rowbytes( const image::buffer_t& pixels)
        {
            return pixels.const_rgba_view().pixels().row_size();
        }

    private:

        // non-copyable
        tile_t( const tile_t&);
        tile_t& operator=( const tile_t&);

        void alloc_tile( int width, int height)
        {
            RAMEN_ASSERT( texture_id_ == 0);

            texture_id_ = gl_gen_texture();
            gl_bind_texture( GL_TEXTURE_2D, texture_id_);

            gl_tex_parameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            gl_tex_parameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            gl_tex_parameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            gl_tex_parameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            gl_tex_image2d( GL_TEXTURE_2D, 0, GL_RGBA32F, width, height, 0, GL_RGBA, GL_FLOAT, 0);
        }

        GLuint texture_id_;
        math::box2i_t area_;
    };

    GLenum texture_unit_;
    math::box2i_t display_window_, data_window_;
    image::buffer_t pixels_;
    boost::ptr_vector<tile_t> tiles_;
};

image_t::image_t( GLenum texture_unit) : texture_unit_( texture_unit) {}

image_t::~image_t()
{
    // do not remove.
    // it's needed by auto_ptr to correctly delete an incomplete type.
}

bool image_t::valid() const
{
    if( !pimpl_.get())
        return false;

    // test more things here...
    return true;
}

void image_t::reset()
{
    pimpl_.reset();
}

void image_t::reset( image::buffer_t pixels,
                     const math::box2i_t& display_window,
                     const math::box2i_t& data_window)
{
    if( pimpl_.get() && pimpl_->update_pixels( pixels, display_window, data_window))
        return;

    create_impl( pixels, display_window, data_window);
}

void image_t::create_impl( const image::buffer_t& pixels,
                           const math::box2i_t& display_window,
                           const math::box2i_t& data_window)
{
    if( display_window.is_empty())
    {
        reset();
        return;
    }

    if( data_window.is_empty())
    {
        reset();
        return;
    }

    pimpl_.reset( new impl( pixels, display_window, data_window));
}

math::box2i_t image_t::display_window() const
{
    if( pimpl_.get())
        return pimpl_->display_window();

    return math::box2i_t();
}

math::box2i_t image_t::data_window() const
{
    if( pimpl_.get())
        return pimpl_->data_window();

    return math::box2i_t();
}

void image_t::draw() const
{
    if( pimpl_.get())
        pimpl_->draw();
}

void image_t::draw_background() const
{
    if( pimpl_.get())
    {
        const math::box2i_t& dw( pimpl_->display_window());

        if( !dw.is_empty())
        {
            gl_begin( GL_QUADS);
                gl_vertices_for_box( dw);
            gl_end();
        }
    }
}

void image_t::frame_display_window() const
{
    if( pimpl_.get())
        frame_rect( pimpl_->display_window());
}

void image_t::frame_data_window() const
{
    if( pimpl_.get())
        frame_rect( pimpl_->data_window());
}

boost::optional<color::rgba_colorf_t> image_t::color_at( const math::point2i_t& p) const
{
    if( pimpl_.get())
        return pimpl_->color_at( p);

    return boost::optional<color::rgba_colorf_t>();
}

// draw utils
void image_t::frame_rect( const math::box2i_t& b) const
{
    if( !b.is_empty())
    {
        gl_begin( GL_LINE_LOOP);
        gl_vertices_for_box( b);
        gl_end();
    }
}

void image_t::gl_vertices_for_box( const math::box2i_t& b) const
{
    gl_vertex2i( b.min.x		, b.min.y);
    gl_vertex2i( b.max.x + 1	, b.min.y);
    gl_vertex2i( b.max.x + 1	, b.max.y + 1);
    gl_vertex2i( b.min.x		, b.max.y + 1);
}

} // viewer
} // ui
} // ramen
