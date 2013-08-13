// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODE_FACTORY_HPP
#define RAMEN_NODE_FACTORY_HPP

#include<utility>
#include<vector>
#include<map>

#include<ramen/core/memory.hpp>

#include<ramen/nodes/node_info.hpp>

namespace ramen
{

/*!
\ingroup nodes
\brief Singleton node factory.
*/
class node_factory_t
{
public:

    static node_factory_t& instance();

    bool register_node( const node_info_t& m);

    void sort_by_menu_item();

    // all node infos
    const std::vector<node_info_t>& registered_nodes() const   { return node_infos_;}
    std::vector<node_info_t>& registered_nodes()               { return node_infos_;}

    // latest versions
    typedef std::map<core::name_t, node_info_t>::const_iterator const_iterator;
    typedef std::map<core::name_t, node_info_t>::iterator       iterator;

    const_iterator latest_versions_begin() const    { return newest_node_infos_.begin();}
    const_iterator latest_versions_end() const	    { return newest_node_infos_.end();}

    iterator latest_versions_begin()    { return newest_node_infos_.begin();}
    iterator latest_versions_end()      { return newest_node_infos_.end();}

    bool is_latest_version( const core::name_t& id) const;

    // creation
    core::auto_ptr_t<node_t> create_by_id( const core::name_t& id, bool ui = false);
    core::auto_ptr_t<node_t> create_by_id_with_version( const core::name_t& id,
                                                        const std::pair<int, int>& version);

private:

    node_factory_t();
    ~node_factory_t();

    // non-copyable
    node_factory_t( const node_factory_t&);
    node_factory_t& operator=( const node_factory_t&);

    std::vector<node_info_t> node_infos_;
    std::map<core::name_t, node_info_t> newest_node_infos_;
};

} // ramen

#endif
