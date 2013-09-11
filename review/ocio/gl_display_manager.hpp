// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_OCIO_GL_DISPLAY_MANAGER_HPP
#define	RAMEN_OCIO_GL_DISPLAY_MANAGER_HPP

#include<utility>

#include<boost/noncopyable.hpp>
#include<boost/shared_ptr.hpp>
#include<boost/function.hpp>

#include<ramen/core/string8.hpp>
#include<ramen/color/color3.hpp>
#include<ramen/color/rgba_color.hpp>

#include<ramen/GL/program.hpp>

#include<ramen/ocio/gl_lut3d.hpp>

namespace ramen
{
namespace ocio
{

class gl_display_manager_t : boost::noncopyable
{
public:

    typedef boost::function<std::pair<core::string8_t, core::string8_t>( int)> get_context_callback_t;

    enum view_channels_t
    {
        view_rgb_channels = 0,
        view_red_channel,
        view_green_channel,
        view_blue_channel,
        view_alpha_channel
    };

    gl_display_manager_t( int lut_size = 32, GLenum texture_num = GL_TEXTURE1);
    gl_display_manager_t( boost::shared_ptr<gl_lut3d_t> lut);

    void set_context_callback( const get_context_callback_t& f);

    void set_display_transform( OCIO::ConstConfigRcPtr config,
                                OCIO::DisplayTransformRcPtr transform);

    color::color3f_t transform( const color::color3f_t& c) const;
    color::rgba_colorf_t transform( const color::rgba_colorf_t& c) const;

    const color::color3f_t& black() const { return lut_->black();}

    view_channels_t view_channels() const		{ return view_channels_;}
    void set_view_channels( view_channels_t v)	{ view_channels_ = v;}

    float exposure() const		{ return exposure_;}
    void set_exposure( float f)	{ exposure_ = f;}

    float gamma() const			{ return gamma_;}
    void set_gamma( float f)	{ gamma_ = f;}

    void activate();
    void deactivate();

private:

    void init();

    static std::pair<core::string8_t, core::string8_t> default_get_ctx( int i);

    OCIO::ConstContextRcPtr get_local_context();

    get_context_callback_t get_ctx_fun_;

    OCIO::ConstProcessorRcPtr processor_;

    gl::program_t program_;
    boost::shared_ptr<gl_lut3d_t> lut_;

    float exposure_;
    float gamma_;
    view_channels_t view_channels_;
};

} // ocio
} // ramen

#endif