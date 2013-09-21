// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/depgraph/exceptions.hpp>

namespace ramen
{
namespace depgraph
{

invalid_dependency::invalid_dependency() {}

const char *invalid_dependency::what() const
{
    return "invalid dependency";
}

dependency_cycle_error::dependency_cycle_error() {}

const char *dependency_cycle_error::what() const
{
    return "dependency cycle";
}

} // depgraph
} // ramen
