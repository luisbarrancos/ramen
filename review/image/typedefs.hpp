// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_IMAGE_TYPEDEFS_HPP
#define RAMEN_IMAGE_TYPEDEFS_HPP

#include<boost/gil/gil_all.hpp>

namespace ramen
{
namespace image
{

typedef boost::gil::rgba32f_pixel_t             pixel_t;
typedef boost::gil::rgba32f_planar_view_t		image_view_t;
typedef boost::gil::rgba32fc_planar_view_t      const_image_view_t;

} // image
} // ramen

#endif
