// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_PLUG_HPP
#define RAMEN_NODES_PLUG_HPP

#include<ramen/config.hpp>

#include<utility>
#include<vector>
#include<algorithm>

#include<boost/flyweight.hpp>
#include<boost/tuple/tuple.hpp>

#include<ramen/assert.hpp>

#include<ramen/core/name.hpp>
#include<ramen/core/string8.hpp>

#include<ramen/color/color3.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{

/*!
\ingroup nodes
\brief Base class for node plugs.
*/
class RAMEN_API node_plug_t
{
public:

    /// Returns this plug color. Used in the UI.
    const color::color3c_t& color() const	{ return color_;}

    /// Returns this plug label. Used in the UI.
    const core::string8_t& ui_label() const	{ return ui_label_;}

protected:

    /// Constructor.
    node_plug_t( const core::string8_t& ui_label,
                 const color::color3c_t& color);

    /// Copy constructor.
    node_plug_t( const node_plug_t& other);

    /// Assignment
    node_plug_t& operator=( const node_plug_t& other);

private:

    color::color3c_t color_;
    boost::flyweight<core::string8_t> ui_label_;
};

/*!
\ingroup nodes
\brief An input plug to which other node plugs can be connected.
*/
class RAMEN_API node_input_plug_t : public node_plug_t
{
public:

    typedef std::pair<node_t*,core::name_t> connection_t;

    node_input_plug_t( const core::string8_t& ui_label,
                       const color::color3c_t& color,
                       bool optional);

    /// Copy constructor.
    node_input_plug_t( const node_input_plug_t& other);

    /// Assignment
    node_input_plug_t& operator=( const node_input_plug_t& other);

    /// Returns if this plug is optional.
    bool optional() const { return optional_;}

    /// Returns true if there's a node connected to this plug.
    bool connected() const	{ return input_ != 0;}

    /// Returns the node connected to this plug, or null.
    const node_t *input_node() const { return input_;}

    /// Returns the node connected to this plug, or null.
    node_t *input_node() { return input_;}

    /// Sets the node and plug this plug is connected to.
    void set_input( node_t *n)
    {
        input_ = n;
    }

    /// Clear this plug connection.
    void clear_input()
    {
        input_ = 0;
    }

private:

    node_t *input_;
    bool optional_;
};

/*!
\ingroup nodes
\brief An output plug to which other node plugs can be connected.
*/
class RAMEN_API node_output_plug_t : public node_plug_t
{
public:

    typedef boost::tuples::tuple<node_t*,int> connection_t;

    /// Constructor.
    node_output_plug_t( node_t *parent,
                        const core::string8_t& ui_label,
                        const color::color3c_t& color);

    /// Copy constructor.
    node_output_plug_t( const node_output_plug_t& other);

    const node_t *parent_node() const { return parent_;}
    node_t *parent_node()             { return parent_;}

    void set_parent_node( node_t *parent)
    {
        RAMEN_ASSERT( parent);

        parent_ = parent;
    }

    /// Adds a connection to this plug.
    void add_output( node_t *n, int port);

    /// Removes a connection to this plug.
    void remove_output( node_t *n, int port);

    typedef std::vector<connection_t>::const_iterator  const_iterator;
    typedef std::vector<connection_t>::iterator        iterator;

    /// Returns a vector of connections from this plug.
    const std::vector<connection_t>& connections() const { return connections_;}

    /// Returns a vector of connections from this plug.
    std::vector<connection_t>& connections() { return connections_;}

private:

    node_t *parent_;
    std::vector<connection_t> connections_;
};

} // ramen

#endif

