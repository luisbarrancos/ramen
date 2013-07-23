// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NAME_HPP
#define RAMEN_NAME_HPP

#include<ramen/core/name_fwd.hpp>

#include<algorithm>
#include<iostream>

namespace ramen
{
namespace core
{

/*!
\ingroup core
\brief Unique string class
*/
class RAMEN_API name_t
{
public:

    name_t();
    explicit name_t( const char *str);

    name_t& operator=( const name_t& other);

    const char *c_str() const;

    bool empty() const;

    std::size_t size() const;

    bool operator==( const name_t& other) const { return data_ == other.data_;}
    bool operator!=( const name_t& other) const { return data_ != other.data_;}
    bool operator<( const name_t& other) const	{ return data_ < other.data_;}

    void swap( name_t& other)
    {
        std::swap( data_, other.data_);
        std::swap( size_, other.size_);
    }

private:

    void init( const char *str);

    const char *data_;
    std::size_t size_;
};

inline void swap( name_t& x, name_t& y)
{
    x.swap( y);
}

inline std::ostream& operator<<( std::ostream& os, const name_t& name)
{
    return os << name.c_str();
}

} // core
} // ramen

#endif
