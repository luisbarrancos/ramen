// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/bezier/algorithm.hpp>

#include<ramen/ggems/nearestpoint.h>

namespace ramen
{
namespace bezier
{

math::point2f_t nearest_point_on_curve( const curve_t<math::point2f_t>& c,
                                        const math::point2f_t& p,
                                        float& t)
{
    Point2 q;
    Point2 cp[4];
    double tt;

    q.x = p.x;
    q.y = p.y;

    for( int i = 0; i < 4; ++i)
    {
        cp[i].x = c[i].x;
        cp[i].y = c[i].y;
    }

    Point2 result = NearestPointOnCurve( q, cp, &tt);
	t = tt;
    return math::point2f_t( result.x, result.y);
}

math::point2f_t nearest_point_on_curve( const curve_t<math::point2f_t>& c,
                                        const math::point2f_t& p)
{
	float t;
	return nearest_point_on_curve( c, p, t);
}

} // bezier
} // ramen
