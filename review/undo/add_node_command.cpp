// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/add_node_command.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/nodes/graph_algorithm.hpp>

#include<ramen/ui/user_interface.hpp>

namespace ramen
{
namespace undo
{

add_node_command_t::add_node_command_t( BOOST_RV_REF( core::auto_ptr_t<node_t>) n,
                                        node_t *src) : command_t( "Add Node"), storage_( n), src_( src)
{
    node_ = storage_.get();
}

add_node_command_t::~add_node_command_t() {}

void add_node_command_t::undo()
{
    if( src_)
        app().document().composition_node().disconnect( src_, node_, 0);

    breadth_first_outputs_search( *node_, boost::bind( &node_t::notify, _1));
    storage_ = app().document().composition_node().release_node( node_);
    app().ui()->node_released( storage_.get());
    command_t::undo();
}

void add_node_command_t::redo()
{
    app().document().composition_node().add_node( storage_);

    if( src_)
    {
        app().document().composition_node().connect( src_, node_, 0);
        breadth_first_outputs_search( *node_, boost::bind( &node_t::notify, _1));
    }

    command_t::redo();
}

} // undo
} // ramen
