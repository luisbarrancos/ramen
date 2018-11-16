// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <functional>

#include <OpenEXR/ImathVec.h>

namespace ramen
{
namespace image
{
typedef std::function<Imath::V2f(const Imath::V2f&)> warp_function_t;

}  // namespace
}  // namespace

