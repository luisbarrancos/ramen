// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/document.hpp>

#include<boost/bind.hpp>
#include<boost/filesystem/fstream.hpp>

#include<ramen/app/application.hpp>

#include<ramen/undo/stack.hpp>

namespace ramen
{

document_t::document_t() : dirty_( false)
{
    world_node_.create_params();
    world_node_.create_manipulators();
    undo_.reset( new undo::stack_t());
}

void document_t::set_file( const boost::filesystem::path& p)
{
    RAMEN_ASSERT( p.empty() || p.is_absolute());

    file_ = p;
    world_node().set_composition_dir( file_.parent_path());
}

const nodes::world_node_t& document_t::world_node() const
{
    return world_node_;
}

nodes::world_node_t& document_t::world_node()
{
    return world_node_;
}

} // ramen
