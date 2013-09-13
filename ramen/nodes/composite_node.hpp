// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITE_NODE_HPP
#define RAMEN_NODES_COMPOSITE_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/nodes/node.hpp>

#include<algorithm>
#include<vector>
#include<map>

#include<boost/bind.hpp>

#include<ramen/core/name.hpp>
#include<ramen/core/memory.hpp>

#include<ramen/containers/ptr_vector.hpp>

#include<ramen/nodes/node_graph_modifier_fwd.hpp>

namespace ramen
{
namespace nodes
{

/**
\ingroup nodes
\brief A node that contains other nodes.
*/
class composite_node_t : public node_t
{
public:

    typedef ramen::containers::ptr_vector_t<node_t> node_container_type;

    typedef node_container_type::iterator       node_iterator;
    typedef node_container_type::const_iterator	const_node_iterator;

    typedef node_container_type::reverse_iterator       reverse_node_iterator;
    typedef node_container_type::const_reverse_iterator	const_reverse_node_iterator;

    typedef node_container_type         node_range_type;
    typedef const node_container_type	const_node_range_type;

    composite_node_t();
    ~composite_node_t();

    boost::signals2::signal<void( node_t*)> node_added;
    boost::signals2::signal<void( node_t*)> node_released;

    // iterators & ranges
    node_iterator nodes_begin() { return nodes_.begin();}
    node_iterator nodes_end()   { return nodes_.end();}

    const_node_iterator nodes_begin() const { return nodes_.begin();}
    const_node_iterator nodes_end() const   { return nodes_.end();}

    node_range_type& nodes()                { return nodes_;}
    const_node_range_type& nodes() const    { return nodes_;}

protected:

    composite_node_t( const composite_node_t& other);

    // will be protected later...

    core::auto_ptr_t<node_t> create_node( const core::name_t& id, bool ui);

    core::auto_ptr_t<node_t> create_node( const core::name_t& id,
                                          const std::pair<int,int>& version);

    core::auto_ptr_t<node_t> create_unknown_node( const core::name_t& id,
                                                  const std::pair<int, int>& version);

    void add_node( BOOST_RV_REF( core::auto_ptr_t<node_t>) n);
    core::auto_ptr_t<node_t> release_node( node_t *n);

private:

    friend class node_graph_modifier_t;
    friend class undo::add_node_command_t;
    friend class undo::remove_node_command_t;
    friend class undo::connect_command_t;
    friend class undo::disconnect_command_t;
    friend class undo::ignore_node_command_t;

    // non-assignable
    void operator=( const composite_node_t& other);

    void emit_node_added_signal( node_t *n);
    void emit_node_released_signal( node_t *n);

    node_container_type nodes_;
};

} // nodes
} // ramen

#endif
