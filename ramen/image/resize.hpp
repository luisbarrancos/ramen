// Copyright (c) 2011 Esteban Tovagliari

#pragma once

#include <ramen/image/filters.hpp>
#include <ramen/image/generic/resize.hpp>

namespace ramen
{
namespace image
{
using generic::resize_box;
using generic::resize_point;

void resize_lanczos3(
    const image::const_image_view_t& src,
    const Imath::Box2i&              src_defined,
    const image::image_view_t&       dst,
    const Imath::Box2i&              dst_area,
    const Imath::V2i&                center,
    const Imath::V2f&                scale);

void resize_mitchell(
    const image::const_image_view_t& src,
    const Imath::Box2i&              src_defined,
    const image::image_view_t&       dst,
    const Imath::Box2i&              dst_area,
    const Imath::V2i&                center,
    const Imath::V2f&                scale);

void resize_catrom(
    const image::const_image_view_t& src,
    const Imath::Box2i&              src_defined,
    const image::image_view_t&       dst,
    const Imath::Box2i&              dst_area,
    const Imath::V2i&                center,
    const Imath::V2f&                scale);

}  // namespace image
}  // namespace ramen
