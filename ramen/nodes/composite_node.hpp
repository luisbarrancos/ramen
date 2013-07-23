// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITE_NODE_HPP
#define RAMEN_NODES_COMPOSITE_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/nodes/node.hpp>

#include<algorithm>
#include<memory>
#include<vector>
#include<map>

#include<boost/bind.hpp>
#include<boost/ptr_container/ptr_vector.hpp>

#include<ramen/core/name.hpp>

#include<ramen/nodes/edge.hpp>

namespace ramen
{

/**
\ingroup nodes
\brief A node that contains other nodes.
*/
class composite_node_t : public node_t
{
public:

    typedef boost::ptr_vector<node_t> node_container_type;

    typedef node_container_type::iterator       node_iterator;
    typedef node_container_type::const_iterator	const_node_iterator;

    typedef node_container_type::reverse_iterator       reverse_node_iterator;
    typedef node_container_type::const_reverse_iterator	const_reverse_node_iterator;

    typedef node_container_type         node_range_type;
    typedef const node_container_type	const_node_range_type;

    typedef std::vector<edge_t>::iterator       edge_iterator;
    typedef std::vector<edge_t>::const_iterator const_edge_iterator;

    typedef std::vector<edge_t>::reverse_iterator       reverse_edge_iterator;
    typedef std::vector<edge_t>::const_reverse_iterator const_reverse_edge_iterator;

    typedef std::vector<edge_t>			edge_range_type;
    typedef const std::vector<edge_t>	const_edge_range_type;

    composite_node_t();
    ~composite_node_t();

    void add_node( std::auto_ptr<node_t> n);
    std::auto_ptr<node_t> release_node( node_t *n);

    void add_edge( const edge_t& e);
    void remove_edge( const edge_t& e);

    // iterators & ranges
    node_iterator nodes_begin() { return nodes_.begin();}
    node_iterator nodes_end()   { return nodes_.end();}

    const_node_iterator nodes_begin() const { return nodes_.begin();}
    const_node_iterator nodes_end() const   { return nodes_.end();}

    node_range_type& nodes()                { return nodes_;}
    const_node_range_type& nodes() const    { return nodes_;}

    edge_iterator edges_begin() { return edges_.begin();}
    edge_iterator edges_end()   { return edges_.end();}

    const_edge_iterator edges_begin() const { return edges_.begin();}
    const_edge_iterator edges_end() const   { return edges_.end();}

    const_edge_range_type& edges() const    { return edges_;}
    edge_range_type& edges()                { return edges_;}

    // connections
    bool can_connect( node_t *src, node_t *dst, int port);
    void connect( node_t *src, node_t *dst, int port);
    void disconnect( node_t *src, node_t *dst, int port);

    // will be protected later...

    std::auto_ptr<node_t> create_node( const std::string& id, bool ui);

    std::auto_ptr<node_t> create_node( const std::string& id,
                                       const std::pair<int,int>& version);

    std::auto_ptr<node_t> create_unknown_node( const std::string& id,
                                               const std::pair<int, int>& version);

protected:

    composite_node_t( const composite_node_t& other);

    virtual void do_begin_interaction();
    virtual void do_end_interaction();

private:

    void operator=( const composite_node_t& other);

    virtual void do_set_frame( float f);

    node_container_type nodes_;
    std::vector<edge_t> edges_;
};

} // ramen

#endif
