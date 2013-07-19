// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_RENDER_RENDER_THREAD_HPP
#define RAMEN_RENDER_RENDER_THREAD_HPP

#include<boost/function.hpp>
#include<boost/thread.hpp>

#include<ramen/render/node_renderer.hpp>

namespace ramen
{
namespace render
{

struct RAMEN_API render_thread_t
{
    render_thread_t();
    ~render_thread_t();

    void init();

    boost::unique_future<bool>& render_image( node_renderer_t& renderer);
    boost::unique_future<bool>& render_image( node_renderer_t& renderer, const Imath::Box2i& roi);

    bool cancelled() const;
    void cancel_render();

private:

    struct impl;
    impl *pimpl_;
};

} // render
} // ramen

#endif
