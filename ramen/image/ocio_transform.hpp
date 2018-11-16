// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;

#include <ramen/image/typedefs.hpp>

namespace ramen
{
namespace image
{
// warning: ocio modifies the pixels in place.!
void ocio_transform(const image_view_t& img, OCIO::ConstProcessorRcPtr proc);

}  // image
}  // ramen

