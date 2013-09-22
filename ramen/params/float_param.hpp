// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAMS_FLOAT_PARAM_HPP
#define	RAMEN_PARAMS_FLOAT_PARAM_HPP

#include<ramen/config.hpp>

#include<ramen/params/param.hpp>

#include<ramen/undo/command.hpp>

namespace ramen
{
namespace params
{

class RAMEN_API float_param_t : public param_t
{
public:

    float_param_t();

protected:

    float_param_t( const float_param_t& other);
    void operator=( const float_param_t& other);

private:

    virtual param_t *do_clone() const;
};

} // params
} // ramen

#endif
