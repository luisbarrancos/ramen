// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/config.hpp>

#include <utility>
#include <vector>
#include <algorithm>

#include <boost/noncopyable.hpp>
#include <boost/tuple/tuple.hpp>

#include <OpenEXR/ImathColor.h>

#include <cassert>

#include <ramen/core/name.hpp>

#include <ramen/nodes/node_fwd.hpp>

namespace ramen
{
/*!
\ingroup nodes
\brief Base class for node plugs.
*/
class RAMEN_API node_plug_t
{
public:
    /// Constructor.
    node_plug_t(const std::string& id, const Imath::Color3c& color, const std::string& tooltip)
    {
        id_      = core::name_t(id.c_str());
        color_   = color;
        tooltip_ = core::name_t(tooltip.c_str());
    }

    /// Copy constructor.
    node_plug_t(const node_plug_t& other)
    : tooltip_(other.tooltip_)
    , color_(other.color_)
    , id_(other.id_)
    {
    }

    /// Returns this plug id.
    const core::name_t& id() const { return id_; }

    /// Returns this plug color. Used in the UI.
    const Imath::Color3c& color() const { return color_; }

    /// Returns this plug tooltip. Used in the UI.
    const core::name_t& tooltip() const { return tooltip_; }

    /// Operator less, for assoc. containers (future).
    bool operator<(const node_plug_t& other) const
    {
        // compare pointers directly.
        return id().c_str() < other.id().c_str();
    }

private:
    core::name_t   id_;
    Imath::Color3c color_;
    core::name_t   tooltip_;
};

/*!
\ingroup nodes
\brief An input plug to which other node plugs can be connected.
*/
class RAMEN_API node_input_plug_t : public node_plug_t
{
public:
    typedef std::pair<node_t*, core::name_t> connection_t;

    node_input_plug_t(const std::string&    id,
                      bool                  optional,
                      const Imath::Color3c& color,
                      const std::string&    tooltip)
    : node_plug_t(id, color, tooltip)
    {
        input_.first = 0;
        optional_    = optional;
    }

    /// Copy constructor.
    node_input_plug_t(const node_input_plug_t& other)
    : node_plug_t(other)
    {
        input_.first = 0;
        optional_    = other.optional();
    }

    /// Returns if this plug is optional.
    bool optional() const { return optional_; }

    /// Returns true if there's a node connected to this plug.
    bool connected() const { return input_.first != 0; }

    /// Returns the node connected to this plug, or null.
    const node_t* input_node() const { return input_.first; }

    /// Returns the node connected to this plug, or null.
    node_t* input_node() { return input_.first; }

    /// Returns the output plug id of the input node this plug is connected to.
    const core::name_t& input_node_out_plug() const { return input_.second; }

    /// Sets the node and plug this plug is connected to.
    void set_input(node_t* n)
    {
        input_.first  = n;
        input_.second = core::name_t("unused");
    }

    /// Sets the node and plug this plug is connected to.
    void set_input(node_t* n, const core::name_t& plug)
    {
        input_.first  = n;
        input_.second = plug;
    }

    /// Clear this plug connection.
    void clear_input() { input_.first = 0; }

private:
    connection_t input_;
    bool         optional_;
};

/*!
\ingroup nodes
\brief An output plug to which other node plugs can be connected.
*/
class RAMEN_API node_output_plug_t : public node_plug_t
{
public:
    typedef boost::tuples::tuple<node_t*, core::name_t, int> connection_t;

    /// Constructor.
    node_output_plug_t(node_t*               parent,
                       const std::string&    id,
                       const Imath::Color3c& color,
                       const std::string&    tooltip);

    virtual ~node_output_plug_t();

    node_output_plug_t* clone() const { return do_clone(); }

    const node_t* parent_node() const { return parent_; }
    node_t*       parent_node() { return parent_; }

    void set_parent_node(node_t* parent)
    {
        assert(parent);

        parent_ = parent;
    }

    /// Adds a connection to this plug.
    void add_output(node_t* n, const core::name_t& plug);

    /// Adds a connection to this plug.
    void add_output(node_t* n, int port);

    /// Removes a connection to this plug.
    void remove_output(node_t* n, const core::name_t& plug);

    /// Removes a connection to this plug.
    void remove_output(node_t* n, int port);

    typedef std::vector<connection_t>::const_iterator const_iterator;
    typedef std::vector<connection_t>::iterator       iterator;

    /// Returns a vector of connections from this plug.
    const std::vector<connection_t>& connections() const { return connections_; }

    /// Returns a vector of connections from this plug.
    std::vector<connection_t>& connections() { return connections_; }

protected:
    node_output_plug_t(const node_output_plug_t& other);

private:
    node_output_plug_t* do_clone() const;

    node_t*                   parent_;
    std::vector<connection_t> connections_;
};

RAMEN_API node_output_plug_t* new_clone(const node_output_plug_t& other);

}  // ramen

