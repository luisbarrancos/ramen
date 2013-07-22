// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/composition_node.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/preferences.hpp>

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

    std::auto_ptr<file_param_t> d( new file_param_t( "Dir"));
    d->set_id( "comp_dir");
    add_param( d);
}

void composition_node_t::do_notify()
{
}

int composition_node_t::start_frame() const
{
    return get_value<float>( param( "start_frame"));
}

int composition_node_t::end_frame() const
{
    return get_value<float>( param( "end_frame"));
}

float composition_node_t::frame() const
{
    return get_value<float>( param( "frame"));
}

bool composition_node_t::autokey() const
{
    return get_value<bool>( param( "autokey"));
}

image::format_t composition_node_t::default_format() const
{
    return get_value<image::format_t>( param( "format"));
}

int composition_node_t::frame_rate() const
{
    return get_value<float>( param( "frame_rate"));
}

render::context_t composition_node_t::current_context( render::render_mode mode) const
{
    render::context_t c;
    c.mode = mode;
    c.composition = 0;
    c.composition_node = const_cast<composition_node_t*>( this);
    c.subsample = 1;
    c.frame = frame();
    c.default_format = default_format();
    return c;
}

} // ramen
