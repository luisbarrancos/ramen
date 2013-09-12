// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPGRAPH_DNODE_HPP
#define RAMEN_DEPGRAPH_DNODE_HPP

#include<ramen/depgraph/dnode_fwd.hpp>

namespace ramen
{
namespace depgraph
{

/**
\ingroup depgraph
\brief dependency node
*/
class RAMEN_API dnode_t
{
public:

    dnode_t();
    virtual ~dnode_t();

protected:

    dnode_t( const dnode_t& other);

private:

    // non-assignable
    dnode_t& operator=( const dnode_t&);
};

} // depgraph
} // ramen

#endif
