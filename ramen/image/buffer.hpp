// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_BUFFER_HPP
#define	RAMEN_IMAGE_BUFFER_HPP

#include<ramen/config.hpp>

#include<boost/shared_array.hpp>
#include<boost/cstdint.hpp>

#include<ramen/math/box2.hpp>

#include<ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{

class RAMEN_API buffer_t
{
public:

    static const unsigned int alignment;

    buffer_t();
    buffer_t( int width, int height, int channels);
    buffer_t( const math::box2i_t& bounds, int channels);

    int width() const	{ return bounds_.size().x + 1;}
    int height() const	{ return bounds_.size().y + 1;}
    int channels() const;

    bool empty() const  { return !pixels_;}
    bool unique() const { return pixels_.unique();}

    void clear();

    const math::box2i_t& bounds() const { return bounds_;}

    // flags
    bool cached() const			{ return cached_pixels_;}
    void set_cached( bool b)	{ cached_pixels_ = true;}

    bool use_disk_cache() const			{ return use_disk_cache_;}
    void set_use_disk_cache( bool b)	{ use_disk_cache_ = b;}

    // const views
    const_image_view_t const_rgba_view() const;
    const_image_view_t const_rgba_subimage_view( const math::box2i_t& area) const;

    // non const views
    image_view_t rgba_view() const;
    image_view_t rgba_subimage_view( const math::box2i_t& area) const;

    bool operator==( const buffer_t& other) const;
    bool operator!=( const buffer_t& other) const;

private:

    void init();

    void alloc_pixels();

    // util
    unsigned char *aligned_ptr() const;

    // for debugging help
    void check_area_inside_image( const math::box2i_t& area) const;

    math::box2i_t bounds_;
    int channels_;
    boost::shared_array<unsigned char> pixels_;
    std::size_t rowbytes_;
    bool cached_pixels_;
    bool use_disk_cache_;
};

} // image
} // ramen

#endif
