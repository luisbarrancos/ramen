// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/image_view.hpp>

#include<QMouseEvent>
#include<QKeyEvent>

#include<ramen/gl/gl.hpp>

#include<ramen/app/application.hpp>

namespace ramen
{
namespace ui
{

image_view_t::image_view_t( QWidget *parent) : QGLWidget( parent)
{
    setFocusPolicy( Qt::ClickFocus);
    setMouseTracking( true);
    first_time_ = true;
}

image_view_t::~image_view_t()
{
}

void image_view_t::initializeGL()
{
    gl::clear_errors();
    gl::clear_color( 0, 0, 0, 0);

    if( first_time_)
    {
        test_gl_extensions();
        first_time_ = false;
    }
    else
    {
        //std::cout << "GL context destroyed!\n";
        // add a call to reinitialize the context?
    }
}

void image_view_t::resizeGL( int w, int h)
{
}

void image_view_t::paintGL()
{
}

void image_view_t::enterEvent( QEvent *event)
{
    event->accept();
}

void image_view_t::leaveEvent( QEvent *event)
{
    event->accept();
}

void image_view_t::keyPressEvent( QKeyEvent *event)
{
    event->accept();
}

void image_view_t::keyReleaseEvent( QKeyEvent *event)
{
    event->accept();
}

void image_view_t::mousePressEvent( QMouseEvent *event)
{
    event->accept();
}

void image_view_t::mouseMoveEvent( QMouseEvent *event)
{
    event->accept();
}

void image_view_t::mouseReleaseEvent( QMouseEvent *event)
{
    event->accept();
}

void image_view_t::test_gl_extensions()
{
    GLenum err = glewInit();

    if( GLEW_OK != err)
        app().fatal_error( "Video card not supported: Error initializing GLEW");

    // test common extensions here
    if( !glewIsSupported("GL_VERSION_2_0"))
        app().fatal_error( "Video card not supported: Not OpenGL 2.0");

    if( !GLEW_ARB_texture_non_power_of_two)
        app().fatal_error( "Video card not supported: Not non power of two textures");

    if( !GLEW_ARB_texture_float)
        app().fatal_error( "Video card not supported: Not float textures");

    if( !GLEW_ARB_fragment_shader)
        app().fatal_error( "Video card not supported: Not fragment programs");

    // OCIO GPU path support
    if( !GLEW_EXT_texture3D)
        app().fatal_error( "Video card not supported: No 3D textures");

    if( !GLEW_ARB_multitexture)
        app().fatal_error( "Video card not supported: No multitexture");

    GLint tx_units;
    glGetIntegerv( GL_MAX_TEXTURE_UNITS, &tx_units);
    if( tx_units < 2)
        app().fatal_error( "Video card not supported: Not enough texture units");
}

} // ui
} // ramen
