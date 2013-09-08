// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_OCIO_GL_LUT3D_HPP
#define	RAMEN_OCIO_GL_LUT3D_HPP

#include<ramen/GL/lut3d.hpp>

#include<OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;

#include<ramen/core/string8.hpp>

#include<ramen/color/color3.hpp>

namespace ramen
{
namespace ocio
{

class gl_lut3d_t : public gl::lut3d_t
{
public:

    gl_lut3d_t( int lut_size = 32, GLenum texture_unit = GL_TEXTURE1);

    void recreate( OCIO::ConstConfigRcPtr config,
                   OCIO::DisplayTransformRcPtr transform,
                   const core::string8_t& fun_name);

    void recreate( OCIO::ConstConfigRcPtr config,
                   OCIO::DisplayTransformRcPtr transform,
                   OCIO::ConstProcessorRcPtr processor,
                   const core::string8_t& fun_name);

    const core::string8_t& lookup_function() const { return lut_fun_;}

    const color::color3f_t& black() const;

private:

    core::string8_t lut_fun_;
    core::string8_t lut_cache_id_;
    color::color3f_t black_;
};

} // ocio
} // ramen

#endif
