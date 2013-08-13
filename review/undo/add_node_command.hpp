// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UNDO_ADD_NODE_COMMAND_HPP
#define	RAMEN_UNDO_ADD_NODE_COMMAND_HPP

#include<ramen/undo/command.hpp>

#include<set>

#include<ramen/nodes/node.hpp>

namespace ramen
{
namespace undo
{

class add_node_command_t : public command_t
{
public:

    add_node_command_t( BOOST_RV_REF( core::auto_ptr_t<node_t>) n, node_t *src = 0);
    virtual ~add_node_command_t();

    virtual void undo();
    virtual void redo();

private:

    node_t *src_, *node_;
    core::auto_ptr_t<node_t> storage_;
};

} // undo
} // ramen

#endif
