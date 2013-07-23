// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/composition.hpp>

namespace ramen
{

composition_t::composition_t()
{
}

// serialization
/*
void composition_t::load_from_file( const boost::filesystem::path& p)
{
    RAMEN_ASSERT( p.is_absolute());

    boost::filesystem::ifstream ifs( p, serialization::yaml_iarchive_t::file_open_mode());

    if( !ifs.is_open() || !ifs.good())
        throw std::runtime_error( std::string( "Couldn't open input file ") + filesystem::file_string( p));

    std::auto_ptr<serialization::yaml_iarchive_t> in( new serialization::yaml_iarchive_t( ifs));

    if( !in->read_composition_header())
        throw std::runtime_error( std::string( "Couldn't open input file ") + filesystem::file_string( p));

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

        std::string id;
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
    std::string src, dst;
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

} // ramen
