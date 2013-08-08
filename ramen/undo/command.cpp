// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/undo/command.hpp>

#include<boost/bind.hpp>
#include<boost/range/algorithm/for_each.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

namespace ramen
{
namespace undo
{

command_t::command_t( const core::string8_t& name) : name_(name), was_dirty_( app().document().dirty())
{
}

command_t::~command_t() {}

const core::string8_t& command_t::name() const { return name_;}

void command_t::set_done( bool b)
{
    done_ = b;

    if( done_)
    app().document().set_dirty( true);
}

void command_t::undo()
{
    app().document().set_dirty( was_dirty_);
    done_ = false;
}

void command_t::redo()
{
    app().document().set_dirty( true);
    done_ = true;
}

composite_command_t::composite_command_t( const core::string8_t& name) : command_t( name)
{
}

void composite_command_t::undo()
{
    boost::range::for_each( commands_, boost::bind( &command_t::undo, _1));
    command_t::undo();
}

void composite_command_t::redo()
{
    boost::range::for_each( commands_, boost::bind( &command_t::redo, _1));
    command_t::redo();
}

void composite_command_t::push_back( std::auto_ptr<command_t> c)
{
    commands_.push_back( c);
}

} // undo
} // ramen
