// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_RENDER_CONTEXT_HPP
#define RAMEN_RENDER_CONTEXT_HPP

#include<ramen/render/context_fwd.hpp>

#include<ramen/image/format.hpp>

#include<boost/function.hpp>

#include<ramen/nodes/node_fwd.hpp>

namespace ramen
{
namespace render
{

enum render_mode
{
    interface_render = 1,
    flipbook_render,
    process_render,
    analisys_render
};

class RAMEN_API context_t
{
public:

    context_t();

    bool render_cancelled() const;

    // data

    composition_node_t *composition_node;

    render_mode mode;

    node_t *result_node;

    double frame;
    int subsample;

    image::format_t default_format;

    int proxy_level;

    int motion_blur_extra_samples;
    float motion_blur_shutter_factor;

    boost::function<bool ()> cancel;

private:

    static bool default_cancel();
};

} // render
} // ramen

#endif
