// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/pointop_node.hpp>

#include<ramen/gil/extension/algorithm/tbb/tbb.hpp>

#include<ramen/params/float_param.hpp>

namespace ramen
{
namespace
{

core::name_t g_red( "red");
core::name_t g_green( "green");
core::name_t g_blue( "blue");

struct channel_mixer_fun
{
    channel_mixer_fun( float mr, float mg, float mb)
    {
        mr_ = mr / (mr + mg + mb);
        mg_ = mg / (mr + mg + mb);
        mb_ = mb / (mr + mg + mb);
    }

    image::pixel_t operator()( const image::pixel_t& src) const
    {
        float val = (src[0] * mr_) + (src[1] * mg_) + (src[2] * mb_);
        return image::pixel_t( val, val, val, src[3]);
    }

private:

    float mr_, mg_, mb_;
};

} // unnamed

class channel_mixer_node_t : public pointop_node_t
{
public:

    static const node_info_t& channel_mixer_node_info();
    virtual const node_info_t *node_info() const;

    channel_mixer_node_t() : pointop_node_t()
    {
        set_name("ch mix");
    }

protected:

    channel_mixer_node_t( const channel_mixer_node_t& other) : pointop_node_t( other) {}
    void operator=( const channel_mixer_node_t&);

private:

    node_t *do_clone() const
    {
        return new channel_mixer_node_t(*this);
    }

    virtual void do_create_params()
    {
        core::auto_ptr_t<float_param_t> p( new float_param_t());
        p->set_name( "Red");
        p->set_id( g_red);
        p->set_default_value( 0.212671f);
        p->set_range( 0, 1);
        p->set_step( 0.05);
        add_param( boost::move( p));

        p.reset( new float_param_t());
        p->set_name( "Green");
        p->set_id( g_green);
        p->set_default_value( 0.715160f);
        p->set_range( 0, 1);
        p->set_step( 0.05);
        add_param( boost::move( p));

        p.reset( new float_param_t());
        p->set_name( "Blue");
        p->set_id( g_blue);
        p->set_default_value( 0.072169f);
        p->set_range( 0, 1);
        p->set_step( 0.05);
        add_param( boost::move( p));
    }

    virtual void do_process( const image::const_image_view_t& src,
                             const image::image_view_t& dst,
                             const render::context_t& context)
    {
        boost::gil::tbb_transform_pixels( src, dst, channel_mixer_fun( get_value<float>( param( g_red)),
                                                                       get_value<float>( param( g_green)),
                                                                       get_value<float>( param( g_blue))));
    }
};

// factory
node_t *create_channel_mixer_node()
{
    return new channel_mixer_node_t();
}

const node_info_t *channel_mixer_node_t::node_info() const
{
    return &channel_mixer_node_info();
}

const node_info_t& channel_mixer_node_t::channel_mixer_node_info()
{
    static bool inited( false);
    static node_info_t info;

    if( !inited)
    {
        info.id = core::name_t( "image.builtin.channel_mixer");
        info.major_version = 1;
        info.minor_version = 0;
        info.menu = "Image";
        info.submenu = "Color";
        info.menu_item = "Channel Mixer";
        info.help = "Converts the input image to gray scale";
        info.create = &create_channel_mixer_node;
        inited = true;
    }

    return info;
}

static bool registered = node_factory_t::instance().register_node( channel_mixer_node_t::channel_mixer_node_info());

} // ramen
