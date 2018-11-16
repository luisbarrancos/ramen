// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/undo/command.hpp>

#include <ramen/app/application.hpp>
#include <ramen/app/document.hpp>

namespace ramen
{
namespace undo
{
command_t::command_t(const std::string& name)
  : name_(name)
  , was_dirty_(app().document().dirty())
{
}
command_t::~command_t() {}

const std::string& command_t::name() const { return name_; }

void command_t::set_done(bool b)
{
    done_ = b;

    if (done_)
        app().document().set_dirty(true);
}

void command_t::undo()
{
    app().document().set_dirty(was_dirty_);
    done_ = false;
}

void command_t::redo()
{
    app().document().set_dirty(true);
    done_ = true;
}

generic_command_t::generic_command_t(
    const std::string&   name,
    const function_type& undo_fun,
    const function_type& redo_fun)
  : command_t(name)
  , undo_(undo_fun)
  , redo_(redo_fun)
{
}

void generic_command_t::undo()
{
    undo_();
    command_t::undo();
}

void generic_command_t::redo()
{
    redo_();
    command_t::redo();
}

composite_command_t::composite_command_t(const std::string& name)
  : command_t(name)
{
}

void composite_command_t::undo()
{
    for (auto& c : commands_)
        c->undo();
    command_t::undo();
}

void composite_command_t::redo()
{
    for (auto& c : commands_)
        c->redo();
    command_t::redo();
}

void composite_command_t::push_back(std::unique_ptr<command_t> c)
{
    commands_.push_back(std::move(c));
}

}  // namespace undo
}  // namespace ramen
