// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_VIEWER_IMAGE_VIEW_HPP
#define	RAMEN_UI_VIEWER_IMAGE_VIEW_HPP

#include<ramen/ui/image_view_fwd.hpp>

#include<ramen/GL/glew.hpp>

#include<QGLWidget>

namespace ramen
{
namespace ui
{

class image_view_t : public QGLWidget
{
    Q_OBJECT

public:

    image_view_t( QWidget *parent = 0);
    virtual ~image_view_t();

protected:

    virtual void initializeGL();
    virtual void resizeGL( int w, int h);
    virtual void paintGL();

    virtual void enterEvent( QEvent *event);
    virtual void leaveEvent( QEvent *event);

    virtual void keyPressEvent( QKeyEvent *event);
    virtual void keyReleaseEvent( QKeyEvent *event);

    virtual void mouseMoveEvent( QMouseEvent *event);
    virtual void mousePressEvent( QMouseEvent *event);
    virtual void mouseReleaseEvent( QMouseEvent *event);

private:

    void test_gl_extensions();

    bool first_time_;
};

} // ui
} // ramen

#endif
