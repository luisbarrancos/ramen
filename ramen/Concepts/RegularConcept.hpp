// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <boost/concept_check.hpp>
#include <boost/swap.hpp>

namespace ramen
{
template <class T>
struct RegularConcept
  : boost::CopyConstructible<T>
  , boost::Assignable<T>
// boost::EqualityComparable<T>
{
    BOOST_CONCEPT_USAGE(RegularConcept) { boost::swap(t, t); }

    T t;
};

}  // namespace ramen
