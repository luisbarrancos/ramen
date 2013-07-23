// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composition_node.hpp>

#include<boost/foreach.hpp>
#include<boost/bind.hpp>
#include<boost/filesystem/operations.hpp>
#include<boost/range/algorithm/for_each.hpp>
#include<boost/range/algorithm/count_if.hpp>
#include<boost/range/algorithm/find_if.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/preferences.hpp>

#include<ramen/nodes/graph_algorithm.hpp>

#include<ramen/params/float_param.hpp>
#include<ramen/params/image_format_param.hpp>
#include<ramen/params/bool_param.hpp>
#include<ramen/params/file_param.hpp>

namespace ramen
{

boost::signals2::signal<void ( node_t*)> composition_node_t::node_created;
boost::signals2::signal<void ( node_t*)> composition_node_t::node_deleted;

composition_node_t::composition_node_t() : composite_node_t()
{
}

composition_node_t::composition_node_t( const composition_node_t& other) : composite_node_t( other)
{
    throw core::not_implemented();
}

node_t *composition_node_t::do_clone() const
{
    return new composition_node_t( *this);
}

void composition_node_t::do_create_params()
{
    std::auto_ptr<float_param_t> p( new float_param_t( "Start Frame"));
    p->set_id( "start_frame");
    p->set_default_value( 1);
    p->set_static( true);
    p->set_round_to_int( true);
    //p->set_secret( true);
    add_param( p);

    p.reset( new float_param_t( "End Frame"));
    p->set_id( "end_frame");
    p->set_default_value( 100);
    p->set_static( true);
    p->set_round_to_int( true);
    //p->set_secret( true);
    add_param( p);

    p.reset( new float_param_t( "Frame"));
    p->set_id( "frame");
    p->set_default_value( 1);
    p->set_static( true);
    //p->set_secret( true);
    p->set_can_undo( false);
    add_param( p);

    std::auto_ptr<image_format_param_t> f( new image_format_param_t( "Default Format"));
    f->set_id( "format");
    //f->set_default_value( app().preferences().default_format());
    add_param( f);

    p.reset( new float_param_t( "Frame Rate"));
    p->set_id( "frame_rate");
    p->set_range( 1, 60);
    p->set_default_value( app().preferences().frame_rate());
    p->set_round_to_int( true);
    p->set_static( true);
    //p->set_secret( true);
    p->set_can_undo( false);
    add_param( p);

    std::auto_ptr<bool_param_t> b( new bool_param_t( "Autokey"));
    b->set_id( "autokey");
    add_param( b);
}

void composition_node_t::do_notify()
{
}

void composition_node_t::do_begin_interaction()
{
    composite_node_t::do_begin_interaction();
}

void composition_node_t::do_end_interaction()
{
    clear_all_notify_dirty_flags();
    composite_node_t::do_end_interaction();
    notify_all_dirty();
}

void composition_t::add_node( std::auto_ptr<node_t> n)
{
    n->set_parent( this);
    n->set_frame( frame());
    render::context_t context = current_context();
    n->calc_format( context);
    n->format_changed();
    composite_node_t::add_node( n);
}

std::auto_ptr<node_t> release_node( node_t *n)
{
    return composite_node_t::release_node( n);
}

std::auto_ptr<node_t> composition_t::release_node( node_t *n)
{
    node_map_.remove( n->name());
    return g_.release_node( n);
}

int composition_node_t::start_frame() const
{
    return get_value<float>( param( "start_frame"));
}

void composition_node_t::set_start_frame( int f)
{
    // TODO: implement this...
}

int composition_node_t::end_frame() const
{
    return get_value<float>( param( "end_frame"));
}

void composition_node_t::set_end_frame( int f)
{
    // TODO: implement this...
}

float composition_node_t::frame() const
{
    return get_value<float>( param( "frame"));
}

void composition_node_t::set_frame( float f)
{
    // TODO: implement this...
    /*
    if( frame_ != f)
    {
        frame_ = f;
        boost::range::for_each( nodes(), boost::bind( &node_t::set_frame, _1, f));
    }
    */
}

bool composition_node_t::autokey() const
{
    return get_value<bool>( param( "autokey"));
}

void composition_node_t::set_autokey( bool b)
{
    // TODO: implement this...
}

image::format_t composition_node_t::default_format() const
{
    return get_value<image::format_t>( param( "format"));
}

void composition_node_t::set_default_format( const image::format_t& f)
{
    // TODO: implement this...
}

int composition_node_t::frame_rate() const
{
    return get_value<float>( param( "frame_rate"));
}

void composition_node_t::set_frame_rate( int f)
{
    // TODO: implement this...
}

render::context_t composition_node_t::current_context( render::render_mode mode) const
{
    render::context_t c;
    c.mode = mode;
    c.composition_node = const_cast<composition_node_t*>( this);
    c.subsample = 1;
    c.frame = frame();
    c.default_format = default_format();
    return c;
}

void composition_node_t::rename_node( node_t *n, const std::string& new_name)
{
    n->set_name( new_name);
}

const boost::filesystem::path& composition_node_t::composition_dir() const
{
    return composition_dir_;
}

void composition_node_t::set_composition_dir( const boost::filesystem::path& dir)
{
    RAMEN_ASSERT( !dir.empty() && dir.is_absolute());

    if( dir == composition_dir())
        return;

    if( !composition_dir().empty())
        convert_all_relative_paths( dir);

    composition_dir_ = dir;
}

void composition_node_t::convert_all_relative_paths( const boost::filesystem::path& new_base)
{
    boost::range::for_each( nodes(), boost::bind( &node_t::convert_relative_paths, _1, composition_dir_, new_base));
}

void composition_node_t::make_all_paths_absolute()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::make_paths_absolute, _1));
}

void composition_node_t::make_all_paths_relative()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::make_paths_relative, _1));
}

boost::filesystem::path composition_node_t::relative_to_absolute( const boost::filesystem::path& p) const
{
    RAMEN_ASSERT( !composition_dir_.empty());
    return filesystem::make_absolute_path( p, composition_dir());
}

boost::filesystem::path composition_node_t::absolute_to_relative( const boost::filesystem::path& p) const
{
    RAMEN_ASSERT( !composition_dir_.empty());
    return filesystem::make_relative_path( p, composition_dir());
}

// selections
void composition_node_t::select_all()
{
    boost::range::for_each( nodes_, boost::bind( &node_t::select, _1, true));
}

void composition_node_t::deselect_all()
{
    boost::range::for_each( nodes_, boost::bind( &node_t::select, _1, false));
}

bool composition_node_t::any_selected() const
{
    return boost::range::find_if( nodes_, boost::bind( &node_t::selected, _1)) != nodes_.end();
}

node_t *composition_node_t::selected_node()
{
    if( boost::range::count_if( nodes_, boost::bind( &node_t::selected, _1)) == 1)
    {
        node_iterator it( nodes_.begin());

        for( ; it != nodes_.end(); ++it)
        {
            if( it->selected())
                return &(*it);
        }
    }

    return 0;
}

void composition_node_t::notify_all_dirty()
{
    for( node_iterator it( nodes_.begin()), ie = nodes_.end(); it != ie; ++it)
    {
        if( it->notify_dirty())
            detail::set_outputs_color( *it, black);
    }

    for( node_iterator it( nodes_.begin()), ie = nodes_.end(); it != ie; ++it)
    {
        if( it->notify_dirty())
            detail::breadth_first_outputs_recursive_search( *it, boost::bind( &node_t::do_notify, _1));
    }
}

void composition_node_t::clear_all_notify_dirty_flags()
{
    boost::range::for_each( nodes_, boost::bind( &node_t::set_notify_dirty, _1, false));
}

} // ramen
