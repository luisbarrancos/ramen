// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPGRAPH_EXCEPTIONS_HPP
#define RAMEN_DEPGRAPH_EXCEPTIONS_HPP

#include<ramen/config.hpp>

#include<ramen/core/exceptions.hpp>

namespace ramen
{
namespace depgraph
{

class RAMEN_API invalid_dependency
{
public:

    invalid_dependency();

    virtual const char *what() const;
};

class RAMEN_API dependency_cycle_error
{
public:

    dependency_cycle_error();

    virtual const char *what() const;
};

} // depgraph
} // ramen

#endif
