// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_NODE_HPP
#define	RAMEN_NODES_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/manipulators/manipulable.hpp>

#include<vector>
#include<string>
#include<memory>
#include<set>
#include<utility>

#include<boost/signals2/signal.hpp>

#include<QWidget>

#include<ramen/core/string8.hpp>

#include<ramen/containers/ptr_vector.hpp>

#include<ramen/math/point2.hpp>
#include<ramen/math/box2.hpp>

#include<ramen/params/param_set.hpp>

#include<ramen/nodes/node_factory.hpp>

#include<ramen/undo/command.hpp>

#include<ramen/ui/palette.hpp>

namespace ramen
{
namespace nodes
{

/*!
\ingroup nodes
\brief A node in Ramen's processing graph.
*/
class RAMEN_API node_t : public manipulators::manipulable_t
{
public:

    //enum flag_bits
    //{
    //};

    virtual const node_info_t *node_info() const { return 0;}

    node_t();
    virtual ~node_t();

    /// Emitted when a node is deleted.
    static boost::signals2::signal<void ( node_t*)> node_deleted;

    /// Emitted when a node is renamed.
    static boost::signals2::signal<void ( node_t*,
                                          const core::string8_t& old_name,
                                          const core::string8_t& new_name)> node_renamed;

    /// Emitted when a node has changed.
    boost::signals2::signal<void ( node_t*)> changed;

    /// Makes a copy of the node.
    node_t *clone() const;

    /// Called for the new node, after being copied.
    virtual void cloned();

    /// Returns the node name.
    const core::string8_t& name() const { return name_;}

    /// Sets the node name.
    void set_name( core::string8_t n);

    /// Returns composite node this node belongs to.
    const composite_node_t *parent() const;

    /// Returns composite node this node belongs to.
    composite_node_t *parent();

    /// Sets the composite node this node belongs to.
    void set_parent( composite_node_t *comp);

    /// Returns the world node this node belongs to.
    const world_node_t *world_node() const;

    /// Returns the world node this node belongs to.
    world_node_t *world_node();

    // ui
    const math::point2f_t& location() const          { return loc_;}
    void set_location( const math::point2f_t& p)     { loc_ = p;}
    void offset_location( const math::vector2f_t& v) { loc_ += v;}

    // params
    /// Creates the params for this node.
    void create_params();

    /// Returns a const reference to the node param_set.
    const params::param_set_t& param_set() const    { return params_;}

    /// Returns a reference to the node param_set.
    params::param_set_t& param_set()				{ return params_;}

    /// Returns a const reference to the param with identifier id.
    const params::param_t& param( const core::name_t& identifier) const;

    /// Returns a reference to the param with identifier id.
    params::param_t& param( const core::name_t& identifier);

    /// Adds a param to this node.
    template<class T>
    void add_param( BOOST_RV_REF( core::auto_ptr_t<T>) p)
    {
        param_set().add_param( p);
    }

    /// Calls a function f for each param.
    virtual void for_each_param( const boost::function<void ( params::param_t*)>& f);

    virtual void param_edit_finished();

    // connections
    virtual bool accept_connection( node_t *src, int port) const;
    void connected( node_t *src, int port);

    /// Creates anim tracks for this node and adds them to root.
    void create_tracks( anim::track_t *root);

    /// Sets the current frame to f.
    void set_frame( float f);

    // user interface
    virtual const char *help_string() const;

    virtual core::auto_ptr_t<QWidget> create_toolbar()
    {
        return core::auto_ptr_t<QWidget>();
    }

    // paths
    virtual void convert_relative_paths( const boost::filesystem::path& old_base,
                                         const boost::filesystem::path& new_base);

    virtual void make_paths_absolute();
    virtual void make_paths_relative();

protected:

    node_t( const node_t& other);
    void operator=( const node_t& other);

    /// Evaluate all params at frame frame.
    void evaluate_params( float frame);

    bool is_valid_, is_identity_;

private:

    /*!
        \brief Customization hook for node_t::clone.
        For subclasses to implement.
    */
    virtual node_t *do_clone() const = 0;

    /*!
        \brief Customization hook for node_t::create_params.
        For subclasses to implement.
    */
    virtual void do_create_params();

    // connections
    virtual void do_connected( node_t *src, int port);

    void reconnect_node();

    /*!
        \brief Customization hook for node_t::create_tracks.
        For subclasses to implement.
    */
    virtual void do_create_tracks( anim::track_t *parent) {}

    /*!
        \brief Customization hook for node_t::set_frame.
        For subclasses to implement.
    */
    virtual void do_set_frame( float t) {}

    // data
    core::string8_t name_;

    params::param_set_t params_;

    boost::uint32_t flags_;
    bool dont_persist_params_;
    math::point2f_t loc_;

    composite_node_t *parent_;
};

/// Makes a copy of a node.
RAMEN_API node_t *new_clone( const node_t& other);

} // nodes
} // ramen

#endif
