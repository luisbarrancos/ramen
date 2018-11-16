// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/GL/gl.hpp>

#ifdef __APPLE__
#include <OpenGL/glu.h>
#else
#include <GL/glu.h>
#endif

namespace ramen
{
void glu_ortho2d(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top);

}  // ramen

