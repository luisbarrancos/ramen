// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_BEZIER_ALGORITHM_HPP
#define	RAMEN_BEZIER_ALGORITHM_HPP

#include<ramen/math/lerp.hpp>

#include<ramen/bezier/curve.hpp>

namespace ramen
{
namespace bezier
{

template<class P>
void split_curve( const curve_t<P,3>& c, double t, curve_t<P,3>& a, curve_t<P,3>& b)
{
    typedef typename curve_t<P,3>::point_type point_type;

    point_type p10, p11, p12, p20, p21, p30;

    for( unsigned int j = 0; j < point_type::dimensions(); ++j)
    {
        p10[j] = math::lerp( c[0][j], c[1][j], t);
        p11[j] = math::lerp( c[1][j], c[2][j], t);
        p12[j] = math::lerp( c[2][j], c[3][j], t);

        p20[j] = math::lerp( p10[j], p11[j], t);
        p21[j] = math::lerp( p11[j], p12[j], t);

        p30[j] = math::lerp( p20[j], p21[j], t);
    }

    a[0] = c[0];
    a[1] = p10;
    a[2] = p20;
    a[3] = p30;

    b[0] = p30;
    b[1] = p21;
    b[2] = p12;
    b[3] = c[3];
}

// Optimized for t = 0.5. It's a very common case.
template<class P>
void split_curve( const curve_t<P,3>& c, curve_t<P,3>& a, curve_t<P,3>& b)
{
    typedef typename curve_t<P,3>::point_type   point_type;

    point_type p10, p11, p12, p20, p21, p30;

    double t = 0.5;

    for( unsigned int j = 0; j < point_type::dimensions(); ++j)
    {
        p10[j] = ( c[0][j] + c[1][j]) * t;
        p11[j] = ( c[1][j] + c[2][j]) * t;
        p12[j] = ( c[2][j] + c[3][j]) * t;
        p20[j] = ( p10[j] + p11[j]) * t;
        p21[j] = ( p11[j] + p12[j]) * t;
        p30[j] = ( p20[j] + p21[j]) * t;
    }

    a[0] = c[0];
    a[1] = p10;
    a[2] = p20;
    a[3] = p30;

    b[0] = p30;
    b[1] = p21;
    b[2] = p12;
    b[3] = c[3];
}

math::point2f_t nearest_point_on_curve( const curve_t<math::point2f_t,3>& c,
                                        const math::point2f_t& p,
                                        float& t);

math::point2f_t nearest_point_on_curve( const curve_t<math::point2f_t,3>& c,
                                        const math::point2f_t& p);

} // bezier
} // ramen

#endif
