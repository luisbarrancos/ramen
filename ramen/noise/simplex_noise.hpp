/*
 * Minimally modified by Esteban Tovagliari
 * Made it more C++ friendly, replaced GLfloats,
 * Removed Doxygen docs, small optimizations,
 * Uses fast float to int conversions for x86 processors,
 */

/*
 * Mesa 3-D graphics library
 * Version:  6.5
 *
 * Copyright (C) 2006  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * BRIAN PAUL BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
 * AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

/*
 * SimplexNoise1234
 * Copyright (c) 2003-2005, Stefan Gustavson
 *
 * Contact: stegu@itn.liu.se
 */

#ifndef RAMEN_NOISE_SIMPLEX_NOISE_HPP
#define RAMEN_NOISE_SIMPLEX_NOISE_HPP

#include <ramen/noise/noise_base.hpp>

#include <boost/noncopyable.hpp>

namespace ramen
{
namespace noise
{
class simplex_noise_t
: public noise_base_t
, boost::noncopyable
{
public:
    simplex_noise_t(int seed = 0);

    float operator()(float x) const;
    float operator()(float x, float y) const;
    float operator()(float x, float y, float z) const;
    float operator()(float x, float y, float z, float w) const;

    float operator()(const Imath::V2f& v) const { return (*this)(v.x, v.y); }
    float operator()(const Imath::V3f& v) const { return (*this)(v.x, v.y, v.z); }
    float operator()(const Imath::V4f& v) const { return (*this)(v.x, v.y, v.z, v.w); }

private:
    float grad1(int hash, float x) const
    {
        int   h    = hash & 15;
        float grad = 1.0f + (h & 7); /* Gradient value 1.0, 2.0, ..., 8.0 */
        if (h & 8)
            grad = -grad;  /* Set a random sign for the gradient */
        return (grad * x); /* Multiply the gradient with the distance */
    }

    float grad2(int hash, float x, float y) const
    {
        int   h = hash & 7;      /* Convert low 3 bits of hash code */
        float u = h < 4 ? x : y; /* into 8 simple gradient directions, */
        float v = h < 4 ? y : x; /* and compute the dot product with (x,y). */
        return ((h & 1) ? -u : u) + ((h & 2) ? -2.0f * v : 2.0f * v);
    }

    float grad3(int hash, float x, float y, float z) const
    {
        int   h = hash & 15;     /* Convert low 4 bits of hash code into 12 simple */
        float u = h < 8 ? x : y; /* gradient directions, and compute dot product. */
        float v = h < 4 ? y : h == 12 || h == 14 ? x : z; /* Fix repeats at h = 12 to 15 */
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }

    float grad4(int hash, float x, float y, float z, float t) const
    {
        int   h = hash & 31;      /* Convert low 5 bits of hash code into 32 simple */
        float u = h < 24 ? x : y; /* gradient directions, and compute dot product. */
        float v = h < 16 ? y : z;
        float w = h < 8 ? z : t;
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v) + ((h & 4) ? -w : w);
    }


    float smoothstep(float v0, float v1, float v) const
    {
        float x = (v - v0) / (v1 - v0);

        if (x > 0.0f)
        {
            if (x < 1.0f)
                return (3.0f - 2.0f * x) * x * x;

            return 1.0f;
        }

        return 0.0f;
    }

    unsigned char perm[512];

    static unsigned char simplex[64][4];
};

}  // noise
}  // ramen

#endif
