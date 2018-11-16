// Copyright (c) 2011 Esteban Tovagliari

#pragma once

#include <ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{
void smart_blur_rgba(const const_image_view_t& src,
                     const image_view_t&       tmp,
                     const image_view_t&       dst,
                     float                     stddevx,
                     float                     stddevy,
                     float                     thereshold);

void smart_blur_rgba(const const_image_view_t& src,
                     const image_view_t&       dst,
                     float                     stddevx,
                     float                     stddevy,
                     float                     thereshold);

}  // namespace
}  // namespace

