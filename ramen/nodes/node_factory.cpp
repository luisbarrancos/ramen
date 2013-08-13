// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/node_factory.hpp>

#include<boost/foreach.hpp>
#include<boost/algorithm/string/predicate.hpp>

#include<ramen/nodes/node.hpp>

#ifndef NDEBUG
    #include<iostream>
#endif

namespace ramen
{
namespace
{

struct compare_menu_items
{
    bool operator()( const node_info_t& a, const node_info_t& b) const
    {
        return a.menu_item < b.menu_item;
    }
};

bool check_node_class( const core::name_t& c)
{
    // TODO: implement this...
    return true;

    //if( boost::starts_with( c, "image."))
    //    return true;

    //return false;
}

} // unnamed

node_factory_t& node_factory_t::instance()
{
    static node_factory_t f;
    return f;
}

node_factory_t::node_factory_t() {}

node_factory_t::~node_factory_t()
{
    for( int i = 0; i < node_infos_.size(); ++i)
    {
        if( !node_infos_[i].first_time_ && node_infos_[i].cleanup)
            node_infos_[i].cleanup();
    }
}

bool node_factory_t::register_node( const node_info_t& m)
{
    if( !check_node_class( m.id))
        throw core::runtime_error( core::make_string( "Registered node class with unknown prefix ", m.id.c_str()));

    RAMEN_ASSERT( m.major_version >= 0);
    RAMEN_ASSERT( m.minor_version >= 0);
    RAMEN_ASSERT( m.create);
    RAMEN_ASSERT( !m.menu.empty());
    RAMEN_ASSERT( !m.submenu.empty());
    RAMEN_ASSERT( !m.menu_item.empty());

    BOOST_FOREACH( const node_info_t& node_info, node_infos_)
    {
        if( node_info.id == m.id && node_info.major_version == m.major_version &&
            node_info.minor_version == m.minor_version)
        {
            throw core::runtime_error( core::make_string( "Duplicated node_info in node factory: ",
                                                          m.id.c_str()));
        }
    }

    node_infos_.push_back( m);

    // if this version of the node is newer that the one we have...
    std::map<core::name_t, node_info_t>::iterator it( newest_node_infos_.find( m.id));

    if( it != latest_versions_end())
    {
        if( m.major_version > it->second.major_version)
            newest_node_infos_.insert( std::make_pair( m.id, m));
        else
        {
            if( m.major_version == it->second.major_version)
            {
                if( m.minor_version > it->second.minor_version)
                    newest_node_infos_.insert( std::make_pair( m.id, m));
            }
        }
    }
    else
        newest_node_infos_.insert( std::make_pair( m.id, m));

    return true;
}

void node_factory_t::sort_by_menu_item()
{
    std::sort( node_infos_.begin(), node_infos_.end(), compare_menu_items());
}

core::auto_ptr_t<node_t> node_factory_t::create_by_id( const core::name_t& id, bool ui)
{
    std::map<core::name_t, node_info_t>::iterator it( newest_node_infos_.find( id));
    core::auto_ptr_t<node_t> n;

    if( it != newest_node_infos_.end())
    {
        try
        {
            if( it->second.first_time_)
            {
                it->second.first_time_ = false;

                if( it->second.init)
                    it->second.init();
            }

            if( it->second.create_gui && ui)
                n.reset( it->second.create_gui());
            else
                n.reset( it->second.create());
        }
        catch( ...)
        {
        }
    }

    return core::auto_ptr_t<node_t>( n.release());
}

core::auto_ptr_t<node_t> node_factory_t::create_by_id_with_version( const core::name_t& id,
                                                                    const std::pair<int, int>& version)
{
    std::vector<node_info_t>::iterator best( node_infos_.end());
    std::vector<node_info_t>::iterator it( node_infos_.begin());
    int best_minor = -1;

    for( ; it != node_infos_.end(); ++it)
    {
        if( it->id == id)
        {
            if( it->major_version == version.first)
            {
                if( it->minor_version > best_minor)
                {
                    best = it;
                    best_minor = it->minor_version;
                }
            }
        }
    }

    if( best_minor < version.second)
        return core::auto_ptr_t<node_t>();

    core::auto_ptr_t<node_t> n;
    if( best != node_infos_.end())
    {
        try
        {
            if( best->first_time_)
            {
                best->first_time_ = false;

                if( best->init)
                    best->init();
            }

            n.reset( best->create());
        }
        catch( ...)
        {
        }
    }

    return core::auto_ptr_t<node_t>( n.release());
}

bool node_factory_t::is_latest_version( const core::name_t& id) const
{
    const_iterator it( newest_node_infos_.find( id));
    return it != latest_versions_end();
}

} // ramen
