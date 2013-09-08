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

#include<ramen/nodes/graph_color.hpp>
#include<ramen/nodes/node_plug.hpp>
#include<ramen/nodes/node_factory.hpp>
#include<ramen/nodes/node_visitor.hpp>

#include<ramen/undo/command.hpp>

#include<ramen/ui/palette.hpp>

namespace ramen
{

/*!
\ingroup nodes
\brief A node in Ramen's processing graph.
*/
class RAMEN_API node_t : public manipulable_t
{
public:

    enum flag_bits
    {
        selected_bit			= 1 << 0,
        ignored_bit				= 1 << 1,
        plugin_error_bit		= 1 << 2,
        active_bit				= 1 << 3,
        context_bit				= 1 << 4,
        cacheable_bit			= 1 << 5,
        autolayout_bit			= 1 << 6,
        notify_dirty_bit		= 1 << 7,
        ui_invisible_bit		= 1 << 8,
        interacting_bit			= 1 << 9
    };

    virtual const node_info_t *node_info() const { return 0;}

    node_t();
    virtual ~node_t();

    // signals

    /// Emitted when a node is deleted.
    static boost::signals2::signal<void ( node_t*)> node_deleted;

    /// Emitted when a node is renamed.
    static boost::signals2::signal<void ( node_t*,
                                          const core::string8_t& old_name,
                                          const core::string8_t& new_name)> node_renamed;

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

    /// Sets the composition this node belongs to.
    void set_parent( composite_node_t *comp);

    /// Returns the composition node this node belongs to.
    const composition_node_t *composition_node() const;

    /// Returns the composition node this node belongs to.
    composition_node_t *composition_node();

    // inputs
    std::size_t num_inputs() const { return inputs_.size();}

    const std::vector<node_input_plug_t>& input_plugs() const { return inputs_;}
    std::vector<node_input_plug_t>& input_plugs()             { return inputs_;}

    const node_t *input( std::size_t i = 0) const;
    node_t *input( std::size_t i = 0);

    template<class T>
    const T *input_as( std::size_t i = 0) const
    {
        return dynamic_cast<const T*>( input( i));
    }

    template<class T>
    T *input_as( std::size_t i = 0)
    {
        return dynamic_cast<T*>( input( i));
    }

    void add_input_plug( const core::string8_t& ui_label,
                         const color::color3c_t& color,
                         bool optional);

    virtual void add_new_input_plug();

    // outputs

    bool has_output_plug() const { return !outputs_.empty();}

    std::size_t num_outputs() const;

    const node_output_plug_t& output_plug() const;
    node_output_plug_t& output_plug();

    void add_output_plug( const core::string8_t& ui_label,
                          const color::color3c_t& color);

    void add_output_plug();

    graph_color_t graph_color() const            { return graph_color_;}
    void set_graph_color( graph_color_t c) const { graph_color_ = c;}

    // visitor
    virtual void accept( node_visitor& v);

    // ui
    const math::point2f_t& location() const          { return loc_;}
    void set_location( const math::point2f_t& p)     { loc_ = p;}
    void offset_location( const math::vector2f_t& v) { loc_ += v;}

    // selection & flags
    bool selected() const;
    void select( bool state);
    void toggle_selection();

    bool plugin_error() const;
    void set_plugin_error( bool b);

    bool autolayout() const;
    void set_autolayout( bool b);

    bool cacheable() const;
    void set_cacheable( bool b);

    bool notify_dirty() const;
    void set_notify_dirty( bool b);

    bool ui_invisible() const;
    void set_ui_invisible( bool b);

    bool is_active() const;
    bool is_context() const;

    bool dont_persist_params() const        { return dont_persist_params_;}
    void set_dont_persist_params( bool b)   { dont_persist_params_ = b;}

    virtual bool autokey() const;
    virtual bool track_mouse() const;

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

    // signals
    boost::signals2::signal<void ( node_t*)> changed;

    void notify();

    // Some parts of Ramen needs access to this, so it's public.
    virtual void do_notify();

    // connections
    virtual bool variable_num_inputs() const { return false;}

    virtual bool accept_connection( node_t *src, int port) const;
    void connected( node_t *src, int port);

    // ignore
    bool ignored() const;
    void set_ignored( bool b);

    // edit
    void begin_active();
    void end_active();

    void begin_context();
    void end_context();

    bool interacting() const;
    void begin_interaction();
    void end_interaction();

    // valid & identity
    bool is_valid() const;
    bool is_identity() const;

    // hash

    virtual bool is_frame_varying() const;

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

    /// Updates widgets associated with this node's params.
    void update_widgets();

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

    virtual void do_begin_active() {}
    virtual void do_end_active() {}

    virtual void do_begin_context() {}
    virtual void do_end_context() {}

    virtual void do_begin_interaction() {}
    virtual void do_end_interaction()	{}

    virtual bool do_is_valid() const;
    virtual bool do_is_identity() const;

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

    std::vector<node_input_plug_t> inputs_;
    containers::ptr_vector_t<node_output_plug_t> outputs_;

    params::param_set_t params_;

    mutable graph_color_t graph_color_;

    boost::uint32_t flags_;
    bool dont_persist_params_;
    math::point2f_t loc_;

    composite_node_t *parent_;
};

/// Makes a copy of a node.
RAMEN_API node_t *new_clone( const node_t& other);

} // ramen

#endif
