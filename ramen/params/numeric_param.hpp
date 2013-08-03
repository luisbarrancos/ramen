// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NUMERIC_PARAM_HPP
#define	RAMEN_NUMERIC_PARAM_HPP

#include<ramen/params/animated_param.hpp>

#include<utility>

#include<ramen/assert.hpp>

#include<ramen/math/vector3.hpp>
#include<ramen/math/box2.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{

class RAMEN_API numeric_param_t : public animated_param_t
{
    Q_OBJECT

public:

    enum numeric_type_t
    {
        absolute = 0,
        relative_x,
        relative_y,
        relative_xy,
        relative_size_x,
        relative_size_y,
        relative_size_xy
    };

    explicit numeric_param_t( const core::string8_t& name);

    bool round_to_int() const { return flags() & round_to_int_bit;}
    void set_round_to_int( bool r);

    numeric_type_t numeric_type() const		{ return type_;}
    void set_numeric_type( numeric_type_t type, int port = -1);

    float relative_to_absolute( float x) const;
    math::vector2f_t relative_to_absolute( const math::vector2f_t& x) const;
    math::box2f_t relative_to_absolute( const math::box2f_t& x) const;

    float absolute_to_relative( float x) const;
    math::vector2f_t absolute_to_relative( const math::vector2f_t& x) const;
    math::box2f_t absolute_to_relative( const math::box2f_t& x) const;

protected:

    numeric_param_t( const numeric_param_t& other);
    void operator=( const numeric_param_t& other);

    float absolute_min() const;
    float absolute_max() const;

    float round( float x) const;
    math::vector2f_t round( const math::vector2f_t& x) const;
    math::vector3f_t round( const math::vector3f_t& x) const;

    void get_scale_and_offset( float& scale, float& offset) const;
    void get_scale_and_offset( math::vector2f_t& scale, math::vector2f_t& offset) const;

private:

    poly_param_value_t relative_to_absolute( const poly_param_value_t& val) const;
    poly_param_value_t absolute_to_relative( const poly_param_value_t& val) const;

    math::box2i_t frame_area() const;

    numeric_type_t type_;
    int depends_on_port_;
};

template<class S>
S get_absolute_value( const param_t& p)
{
    const numeric_param_t *q = dynamic_cast<const numeric_param_t*>( &p);
    RAMEN_ASSERT( q);

    const poly_param_value_t& any( p.value());

    #ifdef NDEBUG
        S v = any.cast<S>();
        return q->relative_to_absolute( v);
    #else
        try
        {
            S v = any.cast<S>();
            return q->relative_to_absolute( v);
        }
        catch( core::bad_cast& e)
        {
            RAMEN_ASSERT( 0 && "Bad cast exception in get_value");
        }
    #endif
}

template<class S>
S get_absolute_value_at_frame( const param_t& p, float frame)
{
    const numeric_param_t *q = dynamic_cast<const numeric_param_t*>( &p);
    RAMEN_ASSERT( q);

    const poly_param_value_t& any( p.value_at_frame( frame));

    #ifdef NDEBUG
        S v = any.cast<S>();
        return q->relative_to_absolute( v);
    #else
        try
        {
            S v = any.cast<S>();
            return q->relative_to_absolute( v);
        }
        catch( core::bad_cast& e)
        {
            RAMEN_ASSERT( 0 && "Bad cast exception in get_value");
        }
    #endif
}

} // ramen

#endif
