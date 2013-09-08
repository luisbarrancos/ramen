// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_GL_LUT3D_HPP
#define	RAMEN_GL_LUT3D_HPP

#include<boost/noncopyable.hpp>
#include<boost/scoped_array.hpp>

#include<ramen/color/color3.hpp>

#include<ramen/GL/gl.hpp>

namespace ramen
{
namespace gl
{

class lut3d_t : boost::noncopyable
{
public:

    lut3d_t( int lut_size = 32, GLenum texture_unit = GL_TEXTURE1);
    ~lut3d_t();

    int lut_size() const        { return lut_size_;}
    color::color3f_t *data()	{ return data_.get();}

	void update_gl_texture();

    void bind();
    void unbind();

private:

	void create_gl_texture();

    GLenum texture_unit_;

    int lut_size_;
    boost::scoped_array<color::color3f_t> data_;

    GLuint texture_id_;
};

} // gl
} // ramen

#endif
