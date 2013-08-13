// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITION_NODE_HPP
#define RAMEN_NODES_COMPOSITION_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/nodes/composite_node.hpp>

#include<ramen/render/context.hpp>

namespace ramen
{

class composition_node_t : public composite_node_t
{
public:

    /// Constructor
    composition_node_t();

    // signals

    /// Emitted when a node is created.
    static boost::signals2::signal<void ( node_t*)> node_created;

    /// Emitted when a node is deleted.
    static boost::signals2::signal<void ( node_t*)> node_deleted;

    void add_node( BOOST_RV_REF( core::auto_ptr_t<node_t>) n);
    core::auto_ptr_t<node_t> release_node( node_t *n);

    int start_frame() const;
    void set_start_frame( int f);

    int end_frame() const;
    void set_end_frame( int f);

    float frame() const;
    void set_frame( float f);

    bool autokey() const;
    void set_autokey( bool b);

    image::format_t default_format() const;
    void set_default_format( const image::format_t& f);

    int frame_rate() const;
    void set_frame_rate( int f);

    render::context_t current_context( render::render_mode mode = render::interface_render) const;

    // misc
    void rename_node( node_t *n, const core::string8_t& new_name);

    const boost::filesystem::path& composition_dir() const;
    void set_composition_dir( const boost::filesystem::path& dir);

    void convert_all_relative_paths( const boost::filesystem::path& new_base);
    void make_all_paths_absolute();
    void make_all_paths_relative();

    boost::filesystem::path relative_to_absolute( const boost::filesystem::path& p) const;
    boost::filesystem::path absolute_to_relative( const boost::filesystem::path& p) const;

    // selections
    void select_all();
    void deselect_all();

    bool any_selected() const;
    node_t *selected_node();

    // paths

    boost::filesystem::path make_absolute_path( const boost::filesystem::path& p,
                                                const boost::filesystem::path& from) const;

    boost::filesystem::path make_relative_path( const boost::filesystem::path& p,
                                                const boost::filesystem::path& from) const;

    boost::filesystem::path convert_relative_path( const boost::filesystem::path& p,
                                                   const boost::filesystem::path& old_base,
                                                   const boost::filesystem::path& new_base) const;

protected:

    composition_node_t( const composition_node_t& other);

private:

    void operator=( const composition_node_t& other);

    virtual node_t *do_clone() const;

    virtual void do_create_params();

    virtual void do_notify();

    virtual void do_begin_interaction();
    virtual void do_end_interaction();

    // notifications
    void notify_all_dirty();
    void clear_all_notify_dirty_flags();

    boost::filesystem::path composition_dir_;

    float_param_t *start_frame_;
    float_param_t *end_frame_;
    float_param_t *frame_;
    image_format_param_t *default_format_;
    float_param_t *frame_rate_;
    bool_param_t *autokey_;
};

} // ramen

#endif
