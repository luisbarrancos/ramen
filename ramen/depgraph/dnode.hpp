// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_DEPGRAPH_DNODE_HPP
#define RAMEN_DEPGRAPH_DNODE_HPP

#include<ramen/depgraph/dnode_fwd.hpp>

#include<vector>

#include<boost/signals2/signal.hpp>

#include<ramen/depgraph/dgraph_fwd.hpp>

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

    bool dirty() const { return dirty_;}
    bool set_dirty( bool b);

    boost::signals2::signal<void( dnode_t*)> became_dirty;

protected:

    dnode_t( const dnode_t& other);

private:

    friend class dgraph_t;

    // non-assignable
    dnode_t& operator=( const dnode_t&);

    bool dirty_;
    std::vector<dnode_t*> inputs_;
    std::vector<dnode_t*> outputs_;
};

} // depgraph
} // ramen

#endif
