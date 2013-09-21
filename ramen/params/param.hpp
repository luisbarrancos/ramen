// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PARAMS_PARAM_HPP
#define RAMEN_PARAMS_PARAM_HPP

#include<ramen/params/param_fwd.hpp>

#include<ramen/depgraph/dnode.hpp>

#include<memory>
#include<sstream>
#include<string>
#include<vector>
#include<utility>

#include<boost/function.hpp>
#include<boost/flyweight.hpp>
#include<boost/filesystem/path.hpp>

#include<ramen/assert.hpp>

#include<ramen/core/name.hpp>
#include<ramen/core/string8.hpp>
#include<ramen/core/flags.hpp>

#include<ramen/math/box2.hpp>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/anim/track_fwd.hpp>

#include<ramen/undo/command.hpp>

namespace ramen
{
namespace params
{

/**
\ingroup params
\brief param class
*/
class RAMEN_API param_t : public depgraph::dnode_t
{
    enum flag_bits
    {
    };

public:

    enum change_reason
    {
        silent_edit = 0,
        user_edited,
        node_edited,
        node_loaded
   };

    /// Constructor.
    param_t();

    /// Destructor.
    virtual ~param_t() {}

    /// Post constructor initialization.
    void init();

    /// Makes a copy of this param.
    param_t *clone() const { return do_clone();}

    /// Returns the param name.
    const core::string8_t& ui_label() const { return ui_label_;}

    /// Sets the param name.
    void set_ui_label( core::string8_t ui_label);

    /// Returns the param id.
    const core::name_t& id() const { return id_;}

    /// Sets the param id.
    void set_id( const core::name_t& identifier);

    /// Returns a const pointer to the param this param belongs to.
    const param_t *parent() const  { return parent_;}

    /// Returns a pointer to the param this param belongs to.
    param_t *parent() { return parent_;}

    /// Sets the param set this param belongs to.
    void set_parent( param_t *parent);

    /// Returns a const pointer to the param set this param belongs to.
    const param_set_t *param_set() const;

    /// Returns a pointer to the param set this param belongs to.
    param_set_t *param_set();

    /// Returns a const pointer to the node this param belongs to.
    const nodes::node_t *node() const;

    /// Returns a pointer to the node this param belongs to.
    nodes::node_t *node();

    /// Returns a const pointer to the composition node this param belongs to.
    const nodes::world_node_t *world_node() const;

    /// Returns a pointer to the composition node this param belongs to.
    nodes::world_node_t *world_node();

    // flags
    // ...

    // toolips
    const core::string8_t& tooltip() const		{ return tooltip_;}
    void set_tooltip( const core::string8_t& t) { tooltip_ = t;}

    // notifications
    void emit_param_changed( change_reason reason);

    // animation
    void create_tracks( anim::track_t *parent);

    // hash
    // ...

    // undo
    core::auto_ptr_t<undo::command_t> create_command();

    // paths
    void convert_relative_paths( const boost::filesystem::path& old_base,
                                 const boost::filesystem::path& new_base);
    void make_paths_absolute();
    void make_paths_relative();

    // util
    void apply_function( const boost::function<void ( param_t*)>& f);

protected:

    param_t( const param_t& other);
    void operator=( const param_t& other);

private:

    friend class param_set_t;

    virtual void do_init();

    virtual param_t *do_clone() const = 0;

    // time and anim
    virtual void do_create_tracks( anim::track_t *parent);

    // undo
    virtual core::auto_ptr_t<undo::command_t> do_create_command();

    // hash
    // ...

    // paths
    virtual void do_convert_relative_paths( const boost::filesystem::path& old_base,
                                            const boost::filesystem::path& new_base);
    virtual void do_make_paths_absolute();
    virtual void do_make_paths_relative();

    // util
    virtual void do_apply_function( const boost::function<void ( param_t*)>& f);

    param_t *parent_;

    core::name_t id_;
    boost::flyweight<core::string8_t> ui_label_;
    boost::flyweight<core::string8_t> tooltip_;
    boost::uint32_t flags_;
};

RAMEN_API param_t *new_clone( const param_t& other);

} // params
} // ramen

#endif
