// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{
void percentile_filter_channel(
    const const_channel_view_t& src,
    const channel_view_t&       dst,
    int                         radius,
    float                       percent);

void percentile_filter_rgba(
    const const_image_view_t& src,
    const image_view_t&       dst,
    int                       radius,
    float                     percent);
void percentile_filter_rgb(
    const const_image_view_t& src,
    const image_view_t&       dst,
    int                       radius,
    float                     percent);
void percentile_filter_alpha(
    const const_image_view_t& src,
    const image_view_t&       dst,
    int                       radius,
    float                     percent);

// void dilate( const const_gray_image_view_t& src, const gray_image_view_t&
// dst, float radius);

}  // namespace image
}  // namespace ramen
