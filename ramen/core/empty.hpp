// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/config.hpp>

#include <boost/operators.hpp>

namespace ramen
{
namespace core
{
/*!
\ingroup core
\brief Empty class.
*/
struct RAMEN_API empty_t : private boost::totally_ordered<empty_t>
{
    bool operator==(const empty_t&) const;
    bool operator<(const empty_t&) const;

    void swap(empty_t&);
};

inline void swap(empty_t&, empty_t&) {}

}  // core
}  // ramen

