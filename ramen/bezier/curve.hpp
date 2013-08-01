// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_BEZIER_CURVE_HPP
#define	RAMEN_BEZIER_CURVE_HPP

#include<cmath>

#include<boost/config.hpp>
#include<boost/static_assert.hpp>

#include<ramen/math/box2.hpp>
#include<ramen/math/matrix33.hpp>

#include<ramen/bezier/bernstein.hpp>

namespace ramen
{
namespace bezier
{

// P is Imath::Vec 2, 3 or a similar class
template<class P = math::point2f_t, int Degree = 3>
class curve_t
{
    BOOST_STATIC_ASSERT( Degree >= 1);

public:

    BOOST_STATIC_CONSTANT( int, degree = Degree);
    BOOST_STATIC_CONSTANT( int, order  = Degree + 1);

    typedef P                               point_type;
    typedef typename P::vector_type         vector_type;
    typedef typename P::value_type          param_type;
    typedef typename P::value_type          coord_type;
    typedef math::box2_t<coord_type>        box_type;
    typedef math::matrix33_t<coord_type>    matrix_type;

    curve_t() {}

    curve_t( const point_type& q0, const point_type& q1)
    {
        BOOST_STATIC_ASSERT( degree == 1);
        p[0] = q0;
        p[1] = q1;
    }

    curve_t( const point_type& q0, const point_type& q1, const point_type& q2)
    {
        BOOST_STATIC_ASSERT( degree == 2);
        p[0] = q0;
        p[1] = q1;
        p[2] = q2;
    }

    curve_t( const point_type& q0, const point_type& q1,
             const point_type& q2, const point_type& q3)
    {
        BOOST_STATIC_ASSERT( degree == 3);
        p[0] = q0;
        p[1] = q1;
        p[2] = q2;
        p[3] = q3;
    }

    const point_type& operator[]( int indx) const
    {
        assert( indx >= 0 && indx < order && "bezier curve operator[], index out of bounds");
        return p[indx];
    }

    point_type& operator[]( int indx)
    {
        assert( indx >= 0 && indx < order && "bezier curve operator[], index out of bounds");
        return p[indx];
    }

    point_type operator()( param_type t) const
    {
        boost::array<param_type, order> B;
        all_bernstein<param_type, degree>( t, B);

        point_type q;

        for( unsigned int j = 0; j < point_type::size_type::value; ++j)
        {
            q[j] = 0;

            for( int i = 0; i < order; ++i)
                q(j) += B[i] * p[i](j);
        }

        return q;
    }

    box_type bounding_box() const
    {
        box_type b;

        for( int i = 0; i < order; ++i)
            b.extend_by( p[i]);

        return b;
    }

	void translate( const vector_type& t)
	{
        for( int i = 0; i < order; ++i)
			p[i] += t;
	}

	void transform( const matrix_type& m)
	{
        for( int i = 0; i < order; ++i)
			p[i]  = m * p[i];
	}

    point_type p[Degree+1];
};

} // bezier
} // ramen

#endif
