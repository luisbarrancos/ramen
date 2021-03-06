// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/undo/command.hpp>

#include <memory>
#include <set>

#include <ramen/nodes/node.hpp>

namespace ramen
{
namespace undo
{
class add_node_command_t : public command_t
{
  public:
    add_node_command_t(std::auto_ptr<node_t> n, node_t* src = 0);
    ~add_node_command_t() override;

    void undo() override;
    void redo() override;

  private:
    node_t *              src_, *node_;
    std::auto_ptr<node_t> storage_;
};

class add_nodes_command_t : public command_t
{
  public:
    add_nodes_command_t();

    void add_node(std::auto_ptr<node_t> n);

    void undo() override;
    void redo() override;

  private:
    std::vector<node_t*>      nodes_;
    boost::ptr_vector<node_t> node_storage_;
};

}  // namespace undo
}  // namespace ramen
