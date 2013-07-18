// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODE_HPP
#define	RAMEN_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/manipulators/manipulable.hpp>

#include<vector>
#include<string>
#include<memory>
#include<set>
#include<utility>

#include<boost/signals2/signal.hpp>

#include<QWidget>

#include<ramen/params/param_set.hpp>

#include<ramen/app/composition_fwd.hpp>

#include<ramen/nodes/graph_color.hpp>
#include<ramen/nodes/node_plug.hpp>
#include<ramen/nodes/node_factory.hpp>
#include<ramen/nodes/node_visitor.hpp>

#include<ramen/render/context.hpp>

#include<ramen/undo/command.hpp>

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

    virtual const node_metaclass_t *metaclass() const { return 0;}

    node_t();
    virtual ~node_t();

    /// Emitted when this node is deleted.
    boost::signals2::signal<void ( node_t*)> deleted;

    /// Makes a copy of the node.
    node_t *clone() const;

    /// Called for the new node, after being copied.
    virtual void cloned();

    /// Returns the node name.
    const std::string& name() const { return name_;}

    /// Sets the node name.
    void set_name( const std::string& n);

    /// Returns the composition this node belongs to.
    const composition_t *composition() const;

    /// Returns the composition this node belongs to.
    composition_t *composition();

    /// Sets the composition this node belongs to.
    virtual void set_composition( composition_t *comp);

    // inputs
    std::size_t num_inputs() const { return inputs_.size();}

    const std::vector<node_input_plug_t>& input_plugs() const { return inputs_;}
    std::vector<node_input_plug_t>& input_plugs()             { return inputs_;}

    int find_input( const core::name_t& id) const;

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

    void add_input_plug( const std::string& id,
                         bool optional,
                         const Imath::Color3c& color,
                         const std::string& tooltip);

    virtual void add_new_input_plug();

    // outputs

    bool has_output_plug() const { return !outputs_.empty();}

    std::size_t num_outputs() const;

    const node_output_plug_t& output_plug() const;
    node_output_plug_t& output_plug();

    const node_t *output( std::size_t i) const;
    node_t *output( std::size_t i);

    void add_output_plug( const std::string& id,
                          const Imath::Color3c& color,
                          const std::string& tooltip);

    graph_color_t graph_color() const            { return graph_color_;}
    void set_graph_color( graph_color_t c) const { graph_color_ = c;}

    // visitor
    virtual void accept( node_visitor& v);

    // ui
    const Imath::V2f& location() const          { return loc_;}
    void set_location( const Imath::V2f& p)     { loc_ = p;}
    void offset_location( const Imath::V2f& v)	{ loc_ += v;}

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
    const param_set_t& param_set() const    { return params_;}

    /// Returns a reference to the node param_set.
    param_set_t& param_set()				{ return params_;}

    /// Returns a const reference to the param with identifier id.
    const param_t& param( const std::string& identifier) const;

    /// Returns a reference to the param with identifier id.
    param_t& param( const std::string& identifier);

    /// Adds a param to this node.
    template<class T>
    void add_param( std::auto_ptr<T> p) { param_set().add_param( p);}

    /// Calls a function f for each param.
    virtual void for_each_param( const boost::function<void ( param_t*)>& f);

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

    void calc_frames_needed( const render::context_t& context);

    const std::vector<std::pair<int, float> >& frames_needed() const
    {
        return frames_needed_;
    }

    std::vector<std::pair<int, float> >& frames_needed()
    {
        return frames_needed_;
    }

    typedef std::vector<std::pair<int, float> >::const_iterator const_frames_needed_iterator;

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
    virtual void clear_hash();

    const hash::generator_t& hash_generator() const	{ return hash_gen_;}
    hash::generator_t& hash_generator()				{ return hash_gen_;}

    std::string hash_str() const;

    const hash::generator_t::digest_type& digest();

    void calc_hash_str( const render::context_t& context);

    virtual bool include_input_in_hash( int num) const;

    virtual bool is_frame_varying() const;

    /// Creates anim tracks for this node and adds them to root.
    void create_tracks( anim::track_t *root);

    /// Sets the current frame to f.
    void set_frame( float f);

    // user interface
    virtual const char *help_string() const;

    virtual std::auto_ptr<QWidget> create_toolbar() { return std::auto_ptr<QWidget>();}

    /// Updates widgets associated with this node's params.
    void update_widgets();

    // paths
    virtual void convert_relative_paths( const boost::filesystem::path& old_base,
                                         const boost::filesystem::path& new_base);

    virtual void make_paths_absolute();
    virtual void make_paths_relative();

    // serialization
    void read( const serialization::yaml_node_t& in, const std::pair<int,int>& version);
    void write( serialization::yaml_oarchive_t& out) const;

protected:

    node_t( const node_t& other);
    void operator=( const node_t& other);

    virtual void do_calc_hash_str( const render::context_t& context);
    void add_needed_frames_to_hash( const render::context_t& context);
    void add_context_to_hash_string( const render::context_t& context);

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

    virtual void do_calc_frames_needed( const render::context_t& context);

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

    /*!
        \brief Customization hook for node_t::read.
        Implement in subclasses to read extra data from node.
    */
    virtual void do_read( const serialization::yaml_node_t& in,
                          const std::pair<int,int>& version);

    /*!
        \brief Customization hook for node_t::write.
        Implement in subclasses to write extra data to out.
    */
    virtual void do_write( serialization::yaml_oarchive_t& out) const;

    // serialization utils
    void write_node_info( serialization::yaml_oarchive_t& out) const;

    /*!
        \brief Customization hook for node_t::update_widgets.
        For subclasses to implement.
    */
    virtual void do_update_widgets() {}

    // data
    std::string name_;

    std::vector<node_input_plug_t> inputs_;
    boost::ptr_vector<node_output_plug_t> outputs_;

    param_set_t params_;

    mutable graph_color_t graph_color_;

    boost::uint32_t flags_;
    bool dont_persist_params_;
    Imath::V2f loc_;
    std::vector<std::pair<int, float> > frames_needed_;

    // composition this node belongs to.
    composition_t *composition_;

    // hash
    hash::generator_t hash_gen_;
};

/// Makes a copy of a node.
RAMEN_API node_t *new_clone( const node_t& other);

} // ramen

#endif
