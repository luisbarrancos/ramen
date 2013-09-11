// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/world_node.hpp>

#include<boost/foreach.hpp>
#include<boost/bind.hpp>
#include<boost/filesystem/operations.hpp>
#include<boost/range/algorithm/for_each.hpp>
#include<boost/range/algorithm/count_if.hpp>
#include<boost/range/algorithm/find_if.hpp>

#include<QDir>

#include<ramen/core/exceptions.hpp>

#include<ramen/string_algo/increment_end_digits.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/preferences.hpp>

#ifndef NDEBUG
    #include<iostream>
#endif

namespace ramen
{
namespace
{

/*
core::name_t g_start_frame( "start_frame");
core::name_t g_end_frame( "end_frame");
core::name_t g_frame( "frame");
core::name_t g_format( "format");
core::name_t g_frame_rate( "frame_rate");
core::name_t g_autokey( "autokey");
*/

// debug stuff
#ifndef NDEBUG
    void log_node_released_signal( node_t *n)
    {
        std::cout << "node released " << n->name() << std::endl;
    }
#endif

} // unnamed

world_node_t::world_node_t() : composite_node_t()
{
    set_name( "composition");

    #ifndef NDEBUG
        node_released.connect( log_node_released_signal);
    #endif

    node_added.connect( boost::bind( &world_node_t::node_was_added, this, _1));
    node_deleted.connect( boost::bind( &world_node_t::node_was_deleted, this, _1));
    node_renamed.connect( boost::bind( &world_node_t::node_was_renamed, this, _1, _2, _3));
}

world_node_t::world_node_t( const world_node_t& other) : composite_node_t( other)
{
    throw core::not_implemented();
}

node_t *world_node_t::do_clone() const
{
    return new world_node_t( *this);
}

void world_node_t::do_create_params()
{
    /*
    core::auto_ptr_t<float_param_t> p( new float_param_t());
    start_frame_ = p.get();
    p->set_ui_label( "Start Frame");
    p->set_id( g_start_frame);
    p->set_default_value( 1);
    p->set_static( true);
    p->set_round_to_int( true);
    //p->set_secret( true);
    add_param( boost::move( p));

    p.reset( new float_param_t());
    end_frame_ = p.get();
    p->set_ui_label( "End Frame");
    p->set_id( g_end_frame);
    p->set_default_value( 100);
    p->set_static( true);
    p->set_round_to_int( true);
    //p->set_secret( true);
    add_param( boost::move( p));

    p.reset( new float_param_t());
    frame_ = p.get();
    p->set_ui_label( "Frame");
    p->set_id( g_frame);
    p->set_default_value( 1);
    p->set_static( true);
    //p->set_secret( true);
    p->set_can_undo( false);
    add_param( boost::move( p));

    core::auto_ptr_t<image_format_param_t> f( new image_format_param_t());
    default_format_ = f.get();
    f->set_ui_label( "Default Format");
    f->set_id( g_format);
    //f->set_default_value( app().preferences().default_format());
    add_param( boost::move( f));

    p.reset( new float_param_t());
    frame_rate_ = p.get();
    p->set_ui_label( "Frame Rate");
    p->set_id( g_frame_rate);
    p->set_range( 1, 60);
    p->set_default_value( app().preferences().frame_rate());
    p->set_round_to_int( true);
    p->set_static( true);
    //p->set_secret( true);
    p->set_can_undo( false);
    add_param( boost::move( p));

    core::auto_ptr_t<bool_param_t> b( new bool_param_t());
    autokey_ = b.get();
    b->set_ui_label( "Autokey");
    b->set_id( g_autokey);
    add_param( boost::move( b));
    */
}

int world_node_t::start_frame() const
{
    //return get_value<float>( *start_frame_);
    return 0;
}

void world_node_t::set_start_frame( int f)
{
   //start_frame_->set_value( f);
}

int world_node_t::end_frame() const
{
    //return get_value<float>( *end_frame_);
}

void world_node_t::set_end_frame( int f)
{
    //end_frame_->set_value( f);
}

float world_node_t::frame() const
{
    //return get_value<float>( *frame_);
    return 0.0f;
}

void world_node_t::set_frame( float f)
{
    /*
    if( f != frame())
    {
        frame_->set_value( f);
        boost::range::for_each( nodes(), boost::bind( &node_t::set_frame, _1, f));
    }
    */
}

bool world_node_t::autokey() const
{
    //return get_value<bool>( *autokey_);
    return false;
}

void world_node_t::set_autokey( bool b)
{
    //autokey_->set_value( b);
}

int world_node_t::frame_rate() const
{
    //return get_value<float>( *frame_rate_);
    return 0;
}

void world_node_t::set_frame_rate( int f)
{
    //frame_rate_->set_value( f);
}

const boost::filesystem::path& world_node_t::composition_dir() const
{
    return composition_dir_;
}

void world_node_t::set_composition_dir( const boost::filesystem::path& dir)
{
    RAMEN_ASSERT( !dir.empty() && dir.is_absolute());

    if( dir == composition_dir())
        return;

    if( !composition_dir().empty())
        convert_all_relative_paths( dir);

    composition_dir_ = dir;
}

void world_node_t::convert_all_relative_paths( const boost::filesystem::path& new_base)
{
    boost::range::for_each( nodes(), boost::bind( &node_t::convert_relative_paths, _1, composition_dir_, new_base));
}

void world_node_t::make_all_paths_absolute()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::make_paths_absolute, _1));
}

void world_node_t::make_all_paths_relative()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::make_paths_relative, _1));
}

boost::filesystem::path world_node_t::relative_to_absolute( const boost::filesystem::path& p) const
{
    RAMEN_ASSERT( !composition_dir_.empty());
    return make_absolute_path( p, composition_dir());
}

boost::filesystem::path world_node_t::absolute_to_relative( const boost::filesystem::path& p) const
{
    RAMEN_ASSERT( !composition_dir_.empty());
    return make_relative_path( p, composition_dir());
}

boost::filesystem::path world_node_t::make_absolute_path( const boost::filesystem::path& p,
                                                                const boost::filesystem::path& from) const
{
    RAMEN_ASSERT( p.is_relative());
    RAMEN_ASSERT( from.is_absolute());

    QDir dir( QString( from.string().c_str()));
    QString fname( QString( p.string().c_str()));
    QString abs_path( QDir::cleanPath( dir.absoluteFilePath( fname)));
    return boost::filesystem::path( abs_path.toStdString());
}

boost::filesystem::path world_node_t::make_relative_path( const boost::filesystem::path& p,
                                                                const boost::filesystem::path& from) const
{
    RAMEN_ASSERT( p.is_absolute());
    RAMEN_ASSERT( from.is_absolute());

    QDir dir( QString( from.string().c_str()));
    QString fname( QString( p.string().c_str()));
    QString rel_path( dir.relativeFilePath( fname));
    return boost::filesystem::path( rel_path.toStdString());
}

boost::filesystem::path world_node_t::convert_relative_path( const boost::filesystem::path& p,
                                                                   const boost::filesystem::path& old_base,
                                                                   const boost::filesystem::path& new_base) const
{
    boost::filesystem::path p0( make_absolute_path( p, old_base));
    return make_relative_path( p0, new_base);
}

const node_t *world_node_t::find_node( const core::string8_t& name) const
{
    nodes_names_map_type::right_const_iterator it = nodes_names_map_.right.find( name);

    if( it != nodes_names_map_.right.end())
        return it->second;

    return 0;
}

node_t *world_node_t::find_node( const core::string8_t& name)
{
    nodes_names_map_type::right_iterator it = nodes_names_map_.right.find( name);

    if( it != nodes_names_map_.right.end())
        return it->second;

    return 0;
}

void world_node_t::make_name_unique( node_t *n)
{
    core::string8_t new_name( n->name());

    while( node_t *nn = find_node( new_name))
        new_name = string_algo::increment_end_digits( new_name);

    if( new_name != n->name())
        n->set_name( new_name);
}

void world_node_t::node_was_added( node_t *n)
{
    RAMEN_ASSERT( n != this);

    nodes_names_map_.insert( nodes_names_map_type::value_type( n, n->name()));
}

void world_node_t::node_was_deleted( node_t *n)
{
    #ifndef NDEBUG
        std::cout << "node deleted " << n->name() << std::endl;
    #endif

    if( nodes_names_map_.left.find( n) != nodes_names_map_.left.end())
        nodes_names_map_.left.erase( n);
}

void world_node_t::node_was_renamed( node_t* n,
                                           const core::string8_t& old_name,
                                           const core::string8_t& new_name)
{
    #ifndef NDEBUG
        std::cout << "node renamed " << old_name << "->" << new_name << std::endl;
    #endif

    if( nodes_names_map_.left.find( n) != nodes_names_map_.left.end())
    {
        nodes_names_map_.left.erase( n);
        nodes_names_map_.insert( nodes_names_map_type::value_type( n, new_name));
    }
}

} // ramen

