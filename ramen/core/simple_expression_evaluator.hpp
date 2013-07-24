// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_SIMPLE_EXPRESSION_EVALUATOR_HPP
#define RAMEN_SIMPLE_EXPRESSION_EVALUATOR_HPP

#include<ramen/config.hpp>

#include<ramen/core/string8.hpp>

#include<boost/optional.hpp>

namespace ramen
{
namespace core
{

class simple_expression_evaluator_t
{
public:

    simple_expression_evaluator_t();
    ~simple_expression_evaluator_t();

    boost::optional<double> operator()( const core::string8_t& str) const;

private:

    // non-copyable
    simple_expression_evaluator_t( const simple_expression_evaluator_t&);
    simple_expression_evaluator_t& operator=( const simple_expression_evaluator_t&);

    struct impl;
    impl *pimpl_;
};

} // util
} // ramen

#endif
