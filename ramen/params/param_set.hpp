// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAM_PARAM_SET_HPP
#define	RAMEN_PARAM_PARAM_SET_HPP

#include<ramen/params/composite_param.hpp>

#include<boost/signals2/signal.hpp>

namespace ramen
{
namespace params
{

/*!
\ingroup params
\brief A container for params, with some extra functionality
*/
class RAMEN_API param_set_t : public composite_param_t
{
public:

    param_set_t( nodes::node_t *n = 0);
    param_set_t( const param_set_t& other);

    const nodes::node_t *node() const    { return node_;}
    nodes::node_t *node()                { return node_;}
    void set_node( nodes::node_t *n)     { node_ = n;}

    boost::signals2::signal<void ( param_t*, param_t::change_reason)> param_changed;

    void for_each_param( const boost::function<void ( param_t*)>& f);

private:

    nodes::node_t *node_;
};

} // params
} // ramen

#endif
