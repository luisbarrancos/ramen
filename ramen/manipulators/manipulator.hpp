// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_MANIPULATOR_HPP
#define RAMEN_MANIPULATOR_HPP

#include<ramen/config.hpp>

#include<string>

#include<ramen/assert.hpp>

#include<ramen/manipulators/manipulable_fwd.hpp>

#include<ramen/color/color3.hpp>

#include<ramen/ui/events.hpp>

namespace ramen
{

/*!
\ingroup manips
\brief Base class for on-screen controls that recieve events and can be drawn.
*/
class RAMEN_API manipulator_t
{
public:

    manipulator_t();
    virtual ~manipulator_t() {}

    const manipulable_t *parent() const { return parent_;}
    manipulable_t *parent()             { return parent_;}
    void set_parent( manipulable_t *p)  { parent_ = p;}

    /// Returns the visibility of this manipulator.
    bool visible() const		{ return visible_;}

    /// Sets the visibility of this manipulator.
    void set_visible( bool b)	{ visible_ = b;}

    /// Returns whether this manipulator is active or not.
    bool active() const			{ return active_;}

    /// Activates / deactivates this manipulator.
    void set_active( bool b)	{ active_ = b;}

    /// Called when the overlay needs redrawing.
    void draw_overlay( const ui::paint_event_t& event) const;

    /// Called when a key_press_event_t is recieved.
    bool key_press_event( const ui::key_press_event_t& event);

    /// Called when a key_release_event_t is recieved.
    void key_release_event( const ui::key_release_event_t& event);

    /// Called when a mouse_enter_event_t is recieved.
    void mouse_enter_event( const ui::mouse_enter_event_t& event);

    /// Called when a mouse_leave_event_t is recieved.
    void mouse_leave_event( const ui::mouse_leave_event_t& event);

    /// Called when a mouse_press_event_t is recieved.
    bool mouse_press_event( const ui::mouse_press_event_t& event);

    /// Called when a mouse_move_event_t is recieved.
    void mouse_move_event( const ui::mouse_move_event_t& event);

    /// Called when a mouse_drag_event_t is recieved.
    void mouse_drag_event( const ui::mouse_drag_event_t& event);

    /// Called when a mouse_release_event_t is recieved.
    void mouse_release_event( const ui::mouse_release_event_t& event);

private:

    // non-copyable
    manipulator_t( const manipulator_t&);
    manipulator_t& operator=( const manipulator_t&);

    /*!
        \brief Customization hook for manipulator_t::draw_overlay.
        Implement in subclasses to draw a visual representation of the manipulator.
    */
    virtual void do_draw_overlay( const ui::paint_event_t& event) const;

    /*!
        \brief Customization hook for manipulator_t::key_press_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual bool do_key_press_event( const ui::key_press_event_t& event);

    /*!
        \brief Customization hook for manipulator_t::key_release_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_key_release_event( const ui::key_release_event_t& event);

    /*!
        \brief Customization hook for manipulator_t::mouse_enter_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_enter_event( const ui::mouse_enter_event_t& event);

    /*!
        \brief Customization hook for manipulator_t::mouse_leave_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_leave_event( const ui::mouse_leave_event_t& event);

    /*!
        \brief Customization hook for manipulator_t::mouse_press_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual bool do_mouse_press_event( const ui::mouse_press_event_t& event);

    /*!
        \brief Customization hook for manipulator_t::mouse_move_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_move_event( const ui::mouse_move_event_t& event);

    /*!
        \brief Customization hook for manipulator_t::mouse_drag_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_drag_event( const ui::mouse_drag_event_t& event);

    /*!
        \brief Customization hook for manipulator_t::mouse_release_event.
        Implement in subclasses if you need to handle this event class.
    */
    virtual void do_mouse_release_event( const ui::mouse_release_event_t& event);

    manipulable_t *parent_;
    bool visible_;
    bool active_;
};

} // ramen

#endif
