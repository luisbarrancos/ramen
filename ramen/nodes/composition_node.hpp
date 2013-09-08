// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_NODES_COMPOSITION_NODE_HPP
#define RAMEN_NODES_COMPOSITION_NODE_HPP

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/nodes/composite_node.hpp>

#include<boost/bimap.hpp>

namespace ramen
{

class composition_node_t : public composite_node_t
{
public:

    /// Constructor
    composition_node_t();

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

    int frame_rate() const;
    void set_frame_rate( int f);

    const boost::filesystem::path& composition_dir() const;
    void set_composition_dir( const boost::filesystem::path& dir);

    void convert_all_relative_paths( const boost::filesystem::path& new_base);
    void make_all_paths_absolute();
    void make_all_paths_relative();

    boost::filesystem::path relative_to_absolute( const boost::filesystem::path& p) const;
    boost::filesystem::path absolute_to_relative( const boost::filesystem::path& p) const;

    // node names
    const node_t *find_node( const core::string8_t& name) const;
    node_t *find_node( const core::string8_t& name);

    void make_name_unique( node_t *n);

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

    friend class node_graph_modifier_t;
    friend class undo::add_node_command_t;
    friend class undo::remove_node_command_t;
    friend class undo::connect_command_t;
    friend class undo::disconnect_command_t;
    friend class undo::ignore_node_command_t;

    // non-assignable
    void operator=( const composition_node_t& other);

    virtual node_t *do_clone() const;

    virtual void do_create_params();

    // signal handlers
    void node_was_added( node_t *n);
    void node_was_deleted( node_t *n);
    void node_was_renamed( node_t* n,
                           const core::string8_t& old_name,
                           const core::string8_t& new_name);

    boost::filesystem::path composition_dir_;

    typedef boost::bimap<node_t*, core::string8_t> nodes_names_map_type;
    nodes_names_map_type nodes_names_map_;
};

} // ramen

#endif
