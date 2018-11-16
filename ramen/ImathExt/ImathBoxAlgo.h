// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/config.hpp>

#include <algorithm>

#include <OpenEXR/ImathBox.h>
#include <OpenEXR/ImathMatrix.h>
#include <OpenEXR/ImathMath.h>

namespace ramen
{
namespace ImathExt
{
template <class T>
bool isInside(
    const Imath::Box<Imath::Vec2<T>>& bounds,
    const Imath::Box<Imath::Vec2<T>>& box)
{
    return bounds.intersects(box.min) && bounds.intersects(box.max);
}

template <class T>
Imath::Box<Imath::Vec2<T>> intersect(
    const Imath::Box<Imath::Vec2<T>>& a,
    const Imath::Box<Imath::Vec2<T>>& b)
{
    if (!(a.min.x > b.max.x || a.max.x < b.min.x || a.min.y > b.max.y ||
          a.max.y < b.min.y))
    {
        return Imath::Box<Imath::Vec2<T>>(
            Imath::Vec2<T>(
                std::max(a.min.x, b.min.x), std::max(a.min.y, b.min.y)),
            Imath::Vec2<T>(
                std::min(a.max.x, b.max.x), std::min(a.max.y, b.max.y)));
    }
    else
        return Imath::Box<Imath::Vec2<T>>();
}

template <class T> Imath::Box<T> offsetBy(const Imath::Box<T>& box, const T& d)
{
    Imath::Box<T> newbox(box);
    newbox.min += d;
    newbox.max += d;
    return newbox;
}

template <class T>
Imath::Box2i roundBox(
    const Imath::Box<Imath::Vec2<T>>& b,
    bool                              round_up = false)
{
    if (b.isEmpty())
        return Imath::Box2i();

    if (round_up)
    {
        return Imath::Box2i(
            Imath::V2i(
                Imath::Math<T>::floor(b.min.x), Imath::Math<T>::floor(b.min.y)),
            Imath::V2i(
                Imath::Math<T>::ceil(b.max.x), Imath::Math<T>::ceil(b.max.y)));
    }
    else
    {
        return Imath::Box2i(
            Imath::V2i(
                Imath::Math<T>::ceil(b.min.x), Imath::Math<T>::ceil(b.min.y)),
            Imath::V2i(
                Imath::Math<T>::floor(b.max.x),
                Imath::Math<T>::floor(b.max.y)));
    }
}

template <class T>
Imath::Box<Imath::Vec2<T>> transform(
    const Imath::Box<Imath::Vec2<T>>& box,
    const Imath::Matrix33<T>&         m)
{
    if (box.isEmpty())
        return box;

    if (m[0][2] == 0 && m[1][2] == 0 && m[2][2] == 1)
    {
        Imath::Box<Imath::Vec2<T>> newBox;

        for (int i = 0; i < 2; i++)
        {
            newBox.min[i] = newBox.max[i] = m[2][i];

            for (int j = 0; j < 2; j++)
            {
                float a = m[j][i] * box.min[j];
                float b = m[j][i] * box.max[j];

                if (a < b)
                {
                    newBox.min[i] += a;
                    newBox.max[i] += b;
                }
                else
                {
                    newBox.min[i] += b;
                    newBox.max[i] += a;
                }
            }
        }

        return newBox;
    }

    Imath::Vec2<T> points[4];
    points[0].x = box.min.x;
    points[0].y = box.min.y;
    points[1].x = box.max.x;
    points[1].y = box.min.y;
    points[2].x = box.max.x;
    points[2].y = box.max.y;
    points[3].x = box.min.x;
    points[3].y = box.max.y;

    Imath::Box<Imath::Vec2<T>> newBox;

    for (int i = 0; i < 4; i++)
        newBox.extendBy(points[i] * m);

    return newBox;
}

RAMEN_API Imath::Box2i transform(
    const Imath::Box2i& box,
    const Imath::M33f&  m,
    bool                round_up = false);
RAMEN_API Imath::Box2i transform(
    const Imath::Box2i& box,
    const Imath::M33d&  m,
    bool                round_up = false);

RAMEN_API Imath::Box2i scale(const Imath::Box2i& b, float s);
RAMEN_API Imath::Box2i scale(const Imath::Box2i& b, float sx, float sy);
RAMEN_API Imath::Box2i scale(
    const Imath::Box2i& b,
    float               sx,
    float               sy,
    int                 xcenter,
    int                 ycenter);

RAMEN_API Imath::Box2f scale(const Imath::Box2f& b, float s);
RAMEN_API Imath::Box2f scale(const Imath::Box2f& b, float sx, float sy);
RAMEN_API Imath::Box2f scale(
    const Imath::Box2f& b,
    float               sx,
    float               sy,
    float               xcenter,
    float               ycenter);

template <class F>  // -> Imath::V2f F::operator()( const Imath::V2f& p)
Imath::Box2f warpBox(const Imath::Box2f& b, F f)
{
    Imath::Box2f box;
    Imath::V2f   p;

    for (int i = b.min.x; i <= b.max.x; ++i)
    {
        box.extendBy(f(Imath::V2f(i, b.min.y)));
        box.extendBy(f(Imath::V2f(i, b.max.y)));
    }

    for (int i = b.min.y; i <= b.max.y; ++i)
    {
        box.extendBy(f(Imath::V2f(b.min.x, i)));
        box.extendBy(f(Imath::V2f(b.max.x, i)));
    }

    return box;
}

template <class F>  // -> Imath::V2f F::operator()( const Imath::V2f& p)
Imath::Box2i warpBox(const Imath::Box2i& b, F f, bool round_up = false)
{
    Imath::Box2f box(Imath::V2f(b.min), Imath::V2f(b.max));
    box = warpBox(box, f);

    if (box.isEmpty())
        return Imath::Box2i();

    return roundBox(box, round_up);
}

}  // namespace ImathExt
}  // namespace ramen

template <class T>
inline std::ostream& operator<<(std::ostream& os, const Imath::Box<T>& b)
{
    os << "[ " << b.min << "; " << b.max << "]";
    return os;
}
