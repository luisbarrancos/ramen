// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_COMPOSITION_HPP
#define	RAMEN_COMPOSITION_HPP

#include<ramen/config.hpp>

namespace ramen
{

/*!
\ingroup app
\brief Composition class.
*/
class composition_t
{
public:

    composition_t();

private:

    composition_t( const composition_t&);
    composition_t& operator=( const composition_t&);
};

} // ramen

#endif
