// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/params/numeric_param.hpp>

#include<ramen/nodes/composition_node.hpp>

namespace ramen
{

numeric_param_t::numeric_param_t( const core::string8_t& name) : animated_param_t( name), type_( absolute) {}

numeric_param_t::numeric_param_t( const numeric_param_t& other) : animated_param_t( other), type_( other.type_)
{
    depends_on_port_ = other.depends_on_port_;
}

void numeric_param_t::set_round_to_int( bool r)
{
    if( r)
        set_flags( flags() | round_to_int_bit);
    else
        set_flags( flags() & ~round_to_int_bit);
}

float numeric_param_t::round( float x) const
{
    if( round_to_int())
        return (int) x;

    return x;
}

math::vector2f_t numeric_param_t::round( const math::vector2f_t& x) const
{
    if( round_to_int())
        return math::vector2f_t( (int) x.x, (int) x.y);

    return x;
}

math::vector3f_t numeric_param_t::round( const math::vector3f_t& x) const
{
    if( round_to_int())
        return math::vector3f_t( (int) x.x, (int) x.y, (int) x.z);

    return x;
}

void numeric_param_t::set_numeric_type( numeric_type_t type, int port)
{
    type_ = type;
    depends_on_port_ = port;
}

math::box2i_t numeric_param_t::frame_area() const
{
    math::box2i_t area;

    if( depends_on_port_ == -1)
    {
        area = node()->full_format();
        ++area.max.x;
        ++area.max.y;
        return area;
    }

    RAMEN_ASSERT( depends_on_port_ < node()->num_inputs());

    if( node()->input( depends_on_port_))
    {
        area =  node()->input( depends_on_port_)->full_format();
        ++area.max.x;
        ++area.max.y;
    }
    else
        area = composition_node()->default_format().area();

    return area;
}

float numeric_param_t::relative_to_absolute( float x) const
{
    switch( type_)
    {
        case relative_x:
            return ( x * frame_area().size().x) + frame_area().min.x;

        case relative_y:
            return ( x * frame_area().size().y) + frame_area().min.y;

        case relative_size_x:
            return x * frame_area().size().x;

        case relative_size_y:
            return x * frame_area().size().y;

        default:
            return x;
    }
}

math::vector2f_t numeric_param_t::relative_to_absolute( const math::vector2f_t& x) const
{
    switch( type_)
    {
        case relative_xy:
            return math::vector2f_t( ( x.x  * frame_area().size().x) + frame_area().min.x,
                                     ( x.y * frame_area().size().y) + frame_area().min.y);

        case relative_size_xy:
            return math::vector2f_t( x.x  * frame_area().size().x,
                                     x.y * frame_area().size().y);

        default:
            return x;
    }
}

math::box2f_t numeric_param_t::relative_to_absolute( const math::box2f_t& x) const
{
    switch( type_)
    {
        case relative_xy:
            return math::box2f_t( math::point2f_t( x.min.x * frame_area().size().x + frame_area().min.x,
                                                   x.min.y * frame_area().size().y + frame_area().min.y),
                                  math::point2f_t( x.max.x * frame_area().size().x + frame_area().min.x,
                                                   x.max.y * frame_area().size().y + frame_area().min.y));

        case relative_size_xy:
            return math::box2f_t( math::point2f_t( x.min.x * frame_area().size().x,
                                                   x.min.y * frame_area().size().y),
                                  math::point2f_t( x.max.x * frame_area().size().x,
                                                   x.max.y * frame_area().size().y));

        default:
            return x;
    }
}

float numeric_param_t::absolute_to_relative( float x) const
{
    switch( type_)
    {
        case relative_x:
            return ( x - frame_area().min.x) / frame_area().size().x;

        case relative_y:
            return ( x - frame_area().min.y) / frame_area().size().y;

        case relative_size_x:
            return x / frame_area().size().x;

        case relative_size_y:
            return x / frame_area().size().y;

        default:
            return x;
    }
}

math::vector2f_t numeric_param_t::absolute_to_relative( const math::vector2f_t& x) const
{
    switch( type_)
    {
        case relative_xy:
            return math::vector2f_t( ( x.x - frame_area().min.x) / frame_area().size().x,
                                     ( x.y - frame_area().min.y) / frame_area().size().y);

        case relative_size_xy:
            return math::vector2f_t( x.x / frame_area().size().x, x.y / frame_area().size().y);

        default:
            return x;
    }
}

math::box2f_t numeric_param_t::absolute_to_relative( const math::box2f_t& x) const
{
    switch( type_)
    {
        case relative_xy:
            return math::box2f_t( math::point2f_t( ( x.min.x - frame_area().min.x) / frame_area().size().x,
                                                   ( x.min.y - frame_area().min.y) / frame_area().size().y),
                                  math::point2f_t( ( x.max.x - frame_area().min.x) / frame_area().size().x,
                                                   ( x.max.y - frame_area().min.y) / frame_area().size().y));

        case relative_size_xy:
            return math::box2f_t( math::point2f_t( x.min.x / frame_area().size().x,
                                                   x.min.y / frame_area().size().y),
                                  math::point2f_t( x.max.x / frame_area().size().x,
                                                   x.max.y / frame_area().size().y));
        default:
            return x;
    }
}

float numeric_param_t::absolute_min() const
{
    if( get_min() == -std::numeric_limits<float>::max())
        return get_min();

    switch( type_)
    {
        case relative_x:
            return ( get_min() * frame_area().size().x) + frame_area().min.x;

        case relative_y:
            return ( get_min() * frame_area().size().y) + frame_area().min.y;

        case relative_xy:
            return ( get_min() * frame_area().size().x) + frame_area().min.x;

        case relative_size_x:
            return get_min() * frame_area().size().x;

        case relative_size_y:
            return get_min() * frame_area().size().y;

        case relative_size_xy:
            return get_min() * frame_area().size().x;

        default:
            return get_min();
    }
}

float numeric_param_t::absolute_max() const
{
    if( get_max() == std::numeric_limits<float>::max())
        return get_max();

    switch( type_)
    {
        case relative_x:
            return ( get_max() * frame_area().size().x) + frame_area().min.x;

        case relative_y:
            return ( get_max() * frame_area().size().y) + frame_area().min.y;

        case relative_xy:
            return ( get_max() * frame_area().size().x) + frame_area().min.x;

        case relative_size_x:
            return get_max() * frame_area().size().x;

        case relative_size_y:
            return get_max() * frame_area().size().y;

        case relative_size_xy:
            return get_max() * frame_area().size().x;

        default:
            return get_max();
    }
}

void numeric_param_t::get_scale_and_offset( float& scale, float& offset) const
{
    switch( type_)
    {
        case relative_x:
            scale = frame_area().size().x;
            offset = frame_area().min.x;
        break;

        case relative_y:
            scale = frame_area().size().y;
            offset = frame_area().min.y;
        break;

        case relative_size_x:
            scale = frame_area().size().x;
            offset = 0;
        break;

        case relative_size_y:
            scale = frame_area().size().y;
            offset = 0;
        break;

        default:
            scale = 1.0f;
            offset = 0.0f;
    }
}

void numeric_param_t::get_scale_and_offset( math::vector2f_t& scale, math::vector2f_t& offset) const
{
    switch( type_)
    {
        case relative_xy:
            scale = math::vector2f_t( frame_area().size().x, frame_area().size().y);
            offset = math::vector2f_t( frame_area().min.x, frame_area().min.y);
        break;

        case relative_size_xy:
            scale = math::vector2f_t( frame_area().size().x, frame_area().size().y);
            offset = math::vector2f_t( 0, 0);
        break;

        default:
            scale = math::vector2f_t( 1, 1);
            offset = math::vector2f_t( 0, 0);
    }
}

poly_param_value_t numeric_param_t::relative_to_absolute( const poly_param_value_t& val) const
{
    try
    {
        float v = val.cast<float>();
        v = relative_to_absolute( v);
        return poly_param_value_t( v);
    }
    catch( core::bad_cast& e) {}

    try
    {
        math::vector2f_t v = val.cast<math::vector2f_t>();
        v = relative_to_absolute( v);
        poly_param_indexable_value_t result( v);
        return core::poly_cast<poly_param_value_t&>( result);
    }
    catch( core::bad_cast& e) {}

    try
    {
        math::box2f_t v = val.cast<math::box2f_t>();
        v = relative_to_absolute( v);
        return poly_param_value_t( v);
    }
    catch( core::bad_cast& e) {}

    RAMEN_ASSERT( 0);
}

poly_param_value_t numeric_param_t::absolute_to_relative( const poly_param_value_t& val) const
{
    try
    {
        float v = val.cast<float>();
        v = absolute_to_relative( v);
        return poly_param_value_t( v);
    }
    catch( core::bad_cast& e) {}

    try
    {
        math::vector2f_t v = val.cast<math::vector2f_t>();
        v = absolute_to_relative( v);
        poly_param_indexable_value_t result( v);
        return core::poly_cast<poly_param_value_t&>( result);
    }
    catch( core::bad_cast& e) {}

    try
    {
        math::box2f_t v = val.cast<math::box2f_t>();
        v = absolute_to_relative( v);
        return poly_param_value_t( v);
    }
    catch( core::bad_cast& e) {}

    RAMEN_ASSERT( 0);
}

} // namespace
