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

#include<QDir>

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
namespace
{

core::name_t g_start_frame( "start_frame");
core::name_t g_end_frame( "end_frame");
core::name_t g_frame( "frame");
core::name_t g_format( "format");
core::name_t g_frame_rate( "frame_rate");
core::name_t g_autokey( "autokey");

} // unnamed

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
    std::auto_ptr<float_param_t> p( new float_param_t());
    start_frame_ = p.get();
    p->set_name( "Start Frame");
    p->set_id( g_start_frame);
    p->set_default_value( 1);
    p->set_static( true);
    p->set_round_to_int( true);
    //p->set_secret( true);
    add_param( p);

    p.reset( new float_param_t());
    end_frame_ = p.get();
    p->set_name( "End Frame");
    p->set_id( g_end_frame);
    p->set_default_value( 100);
    p->set_static( true);
    p->set_round_to_int( true);
    //p->set_secret( true);
    add_param( p);

    p.reset( new float_param_t());
    frame_ = p.get();
    p->set_name( "Frame");
    p->set_id( g_frame);
    p->set_default_value( 1);
    p->set_static( true);
    //p->set_secret( true);
    p->set_can_undo( false);
    add_param( p);

    std::auto_ptr<image_format_param_t> f( new image_format_param_t());
    default_format_ = f.get();
    f->set_name( "Default Format");
    f->set_id( g_format);
    //f->set_default_value( app().preferences().default_format());
    add_param( f);

    p.reset( new float_param_t());
    frame_rate_ = p.get();
    p->set_name( "Frame Rate");
    p->set_id( g_frame_rate);
    p->set_range( 1, 60);
    p->set_default_value( app().preferences().frame_rate());
    p->set_round_to_int( true);
    p->set_static( true);
    //p->set_secret( true);
    p->set_can_undo( false);
    add_param( p);

    std::auto_ptr<bool_param_t> b( new bool_param_t());
    autokey_ = b.get();
    b->set_name( "Autokey");
    b->set_id( g_autokey);
    add_param( b);
}

void composition_node_t::do_notify() {}

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

void composition_node_t::add_node( std::auto_ptr<node_t> n)
{
    n->set_parent( this);
    n->set_frame( frame());
    render::context_t context = current_context();
    n->calc_format( context);
    n->format_changed();
    composite_node_t::add_node( n);
}

std::auto_ptr<node_t> composition_node_t::release_node( node_t *n)
{
    return composite_node_t::release_node( n);
}

int composition_node_t::start_frame() const
{
    return get_value<float>( *start_frame_);
}

void composition_node_t::set_start_frame( int f)
{
    start_frame_->set_value( f);
}

int composition_node_t::end_frame() const
{
    return get_value<float>( *end_frame_);
}

void composition_node_t::set_end_frame( int f)
{
    end_frame_->set_value( f);
}

float composition_node_t::frame() const
{
    return get_value<float>( *frame_);
}

void composition_node_t::set_frame( float f)
{
    if( f != frame())
    {
        frame_->set_value( f);
        boost::range::for_each( nodes(), boost::bind( &node_t::set_frame, _1, f));
    }
}

bool composition_node_t::autokey() const
{
    return get_value<bool>( *autokey_);
}

void composition_node_t::set_autokey( bool b)
{
    autokey_->set_value( b);
}

image::format_t composition_node_t::default_format() const
{
    return get_value<image::format_t>( *default_format_);
}

void composition_node_t::set_default_format( const image::format_t& f)
{
    default_format_->set_value( f);
}

int composition_node_t::frame_rate() const
{
    return get_value<float>( *frame_rate_);
}

void composition_node_t::set_frame_rate( int f)
{
    frame_rate_->set_value( f);
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

void composition_node_t::rename_node( node_t *n, const core::string8_t& new_name)
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
    return make_absolute_path( p, composition_dir());
}

boost::filesystem::path composition_node_t::absolute_to_relative( const boost::filesystem::path& p) const
{
    RAMEN_ASSERT( !composition_dir_.empty());
    return make_relative_path( p, composition_dir());
}

boost::filesystem::path composition_node_t::make_absolute_path( const boost::filesystem::path& p,
                                                                const boost::filesystem::path& from) const
{
    RAMEN_ASSERT( p.is_relative());
    RAMEN_ASSERT( from.is_absolute());

    QDir dir( QString( from.string().c_str()));
    QString fname( QString( p.string().c_str()));
    QString abs_path( QDir::cleanPath( dir.absoluteFilePath( fname)));
    return boost::filesystem::path( abs_path.toStdString());
}

boost::filesystem::path composition_node_t::make_relative_path( const boost::filesystem::path& p,
                                                                const boost::filesystem::path& from) const
{
    RAMEN_ASSERT( p.is_absolute());
    RAMEN_ASSERT( from.is_absolute());

    QDir dir( QString( from.string().c_str()));
    QString fname( QString( p.string().c_str()));
    QString rel_path( dir.relativeFilePath( fname));
    return boost::filesystem::path( rel_path.toStdString());
}

boost::filesystem::path composition_node_t::convert_relative_path( const boost::filesystem::path& p,
                                                                   const boost::filesystem::path& old_base,
                                                                   const boost::filesystem::path& new_base) const
{
    boost::filesystem::path p0( make_absolute_path( p, old_base));
    return make_relative_path( p0, new_base);
}

// selections
void composition_node_t::select_all()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::select, _1, true));
}

void composition_node_t::deselect_all()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::select, _1, false));
}

bool composition_node_t::any_selected() const
{
    return boost::range::find_if( nodes(), boost::bind( &node_t::selected, _1)) != nodes().end();
}

node_t *composition_node_t::selected_node()
{
    if( boost::range::count_if( nodes(), boost::bind( &node_t::selected, _1)) == 1)
    {
        node_iterator it( nodes().begin());

        for( ; it != nodes().end(); ++it)
        {
            if( it->selected())
                return &(*it);
        }
    }

    return 0;
}

void composition_node_t::notify_all_dirty()
{
    for( node_iterator it( nodes().begin()), ie = nodes().end(); it != ie; ++it)
    {
        if( it->notify_dirty())
            detail::set_outputs_color( *it, black);
    }

    for( node_iterator it( nodes().begin()), ie = nodes().end(); it != ie; ++it)
    {
        if( it->notify_dirty())
            detail::breadth_first_outputs_recursive_search( *it, boost::bind( &node_t::do_notify, _1));
    }
}

void composition_node_t::clear_all_notify_dirty_flags()
{
    boost::range::for_each( nodes(), boost::bind( &node_t::set_notify_dirty, _1, false));
}

} // ramen

// serialization
/*
void composition_t::load_from_file( const boost::filesystem::path& p)
{
    RAMEN_ASSERT( p.is_absolute());

    boost::filesystem::ifstream ifs( p, serialization::yaml_iarchive_t::file_open_mode());

    if( !ifs.is_open() || !ifs.good())
        throw std::runtime_error( core::string8_t( "Couldn't open input file ") + filesystem::file_string( p));

    std::auto_ptr<serialization::yaml_iarchive_t> in( new serialization::yaml_iarchive_t( ifs));

    if( !in->read_composition_header())
        throw std::runtime_error( core::string8_t( "Couldn't open input file ") + filesystem::file_string( p));

    set_composition_dir( p.parent_path());
    read( *in);
}
*/
/*
void composition_t::read( serialization::yaml_iarchive_t& in)
{
    in.get_optional_value( "start_frame", start_frame_);
    in.get_optional_value( "end_frame", end_frame_);
    in.get_optional_value( "frame", frame_);
    in.get_optional_value( "autokey", autokey_);
    in.get_optional_value( "format", default_format_);

    read_nodes( in);
    read_edges( in);

    BOOST_FOREACH( node_t& n, nodes_)
        added_( &n);

    BOOST_FOREACH( node_t& n, nodes_)
        n.for_each_param( boost::bind( &param_t::emit_param_changed, _1, param_t::node_loaded));
}

void composition_t::read_nodes( serialization::yaml_iarchive_t& in)
{
    serialization::yaml_node_t node = in.get_node( "nodes");

    for( int i = 0; i < node.size(); ++i)
        read_node( node[i]);
}

void composition_t::read_node( const serialization::yaml_node_t& node)
{
    try
    {
        serialization::yaml_node_t class_node( node.get_node( "class"));

        core::string8_t id;
        std::pair<int,int> version;

        class_node[0] >> id;
        class_node[1] >> version.first;
        class_node[2] >> version.second;

        std::auto_ptr<node_t> p( create_node( id, version));

        if( !p.get())
        {
            node.error_stream() << "Error creating node: " << id << "\n";
            return;
        }

        p->set_composition( this); // some nodes needs this set early...

        p->read( node, version);
        p->set_frame( frame_);

        render::context_t context = current_context();
        p->calc_format( context);
        p->format_changed();

        node_map_.insert( p.get());
        g_.add_node( p);
    }
    catch( YAML::Exception& e)
    {
        node.error_stream() << "Yaml exception: " << e.what() << " while reading node\n";
    }
    catch( std::runtime_error& e)
    {
        node.error_stream() << "Error reading node\n";
    }
}
*/

/*
void composition_t::read_edges( const serialization::yaml_iarchive_t& in)
{
    serialization::yaml_node_t node = in.get_node( "edges");

    for( int i = 0; i < node.size(); ++i)
        read_edge( node[i]);
}

void composition_t::read_edge( const serialization::yaml_node_t& node)
{
    core::string8_t src, dst;
    int port;

    node[0] >> src;
    node[1] >> dst;
    node[2] >> port;

    node_t *src_ptr = find_node( src);
    node_t *dst_ptr = find_node( dst);

    if( !src_ptr)
    {
        node.error_stream() << "Error: Couldn't find node " << src << "\n";
        return;
    }

    if( !dst_ptr)
    {
        node.error_stream() << "Error: Couldn't find node " << dst << "\n";
        return;
    }

    // TODO: fix this. (est.)
    // if any of the nodes is unknown, add the needed plugs.
    //if( src_ptr->plugin_error() && !src_ptr->has_output_plug())
    //	src_ptr->add_output_plug();

    if( dst_ptr->plugin_error())
    {
        while( dst_ptr->num_inputs() < ( port + 1))
            dst_ptr->add_new_input_plug();
    }

    if( can_connect( src_ptr, dst_ptr, port))
    {
        connect( src_ptr, dst_ptr, port);
        dst_ptr->notify();
    }
    else
        node.error_stream() << "Can't connect " << src_ptr->name() << " to " << dst_ptr->name() << "\n";
}

void composition_t::write( serialization::yaml_oarchive_t& out) const
{
    out << YAML::Key << "start_frame"   << YAML::Value << start_frame_;
    out << YAML::Key << "end_frame"     << YAML::Value << end_frame_;
    out << YAML::Key << "frame"         << YAML::Value << frame_;
    out << YAML::Key << "autokey"       << YAML::Value << autokey_;
    out << YAML::Key << "format"        << YAML::Value << default_format_;
    out.check_errors();

    out << YAML::Key << "nodes" << YAML::Value;
        out.begin_seq();
            boost::range::for_each( nodes_, boost::bind( &node_t::write, _1, boost::ref( out)));
        out.end_seq();

    out << YAML::Key << "edges" << YAML::Value;
        out.begin_seq();
            boost::range::for_each( edges(), boost::bind( &composition_t::write_edge, this, boost::ref( out), _1));
        out.end_seq();
}

void composition_t::write_edge( serialization::yaml_oarchive_t& out, const edge_t& e) const
{
    out.flow();
    out.begin_seq();
        out << e.src->name() << e.dst->name() << e.port;
    out.end_seq();
}
*/
