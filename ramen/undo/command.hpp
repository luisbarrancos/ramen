// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UNDO_COMMAND_HPP
#define RAMEN_UNDO_COMMAND_HPP

#include<ramen/undo/command_fwd.hpp>

#include<ramen/core/memory.hpp>
#include<ramen/core/string8.hpp>

#include<ramen/containers/ptr_vector.hpp>

namespace ramen
{
namespace undo
{

/*!
\ingroup undo
\brief Base class for actions that can be undone and redone.
*/
class command_t
{
public:

    /// Constructor.
    command_t();

    /*!
     Constructor.
     \param name Command name.
     */
    explicit command_t( core::string8_t name);

    /// Destructor.
    virtual ~command_t();

    /// Returns this command name.
    const core::string8_t& name() const;

    /// Sets this command name.
    void set_name( core::string8_t name);

    /// Returns true if this command is already executed.
    bool done() const { return done_;}

    /// Sets if this command is executed.
    void set_done( bool b);

    /// For composite commands, returns true if this command is empty.
    virtual bool empty() const { return false;}

    /// Undoes this command.
    virtual void undo();

    /// Redoes this command.
    virtual void redo();

protected:

    core::string8_t name_;
    bool was_dirty_;
    bool done_;
};

class composite_command_t : public command_t
{
public:

    /// Constructor.
    explicit composite_command_t( core::string8_t name);

    /// Undoes this command.
    virtual void undo();

    /// Redoes this command.
    virtual void redo();

    /// Returns true if this command is empty.
    virtual bool empty() const { return commands_.empty();}

    /// Adds a child command to this command.
    void push_back( BOOST_RV_REF( core::auto_ptr_t<command_t>) c);

protected:

    containers::ptr_vector_t<command_t> commands_;
};

} // undo
} // ramen

#endif
