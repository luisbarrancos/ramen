// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/render/render_thread.hpp>

#include<boost/bind.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace render
{

struct render_thread_t::impl
{
    impl()
    {
        // start our thread
        finish = false;
        ready = false;
        do_work = false;
        thread = boost::thread( &impl::thread_loop, this);
    }

    ~impl()
    {
        boost::unique_lock<boost::mutex> lock( mutex);
        finish = true;
        ready = true;
        do_work = false;
        lock.unlock();
        cond.notify_one();

        // wait for our thread to exit
        thread.join();
    }

    boost::unique_future<bool>& run_function( const boost::function<void()>& f)
    {
        RAMEN_ASSERT( !do_work && "render_thread_t is not reentrant");
        boost::unique_lock<boost::mutex> lock( mutex);
        cancel = false;
        ready = true;
        do_work = true;
        task = boost::packaged_task<bool>( boost::bind( &impl::call_fun, this, f));
        future = task.get_future();
        lock.unlock();
        cond.notify_one();
        return future;
    }

private:

    void thread_loop()
    {
        while( 1)
        {
            boost::unique_lock<boost::mutex> lock( mutex);

            while( !ready)
                cond.wait( lock);

            if( finish)
                break;

            if( do_work)
            {
                do_work = false;
                ready = false;
                task();
            }
        }
    }

    bool call_fun( const boost::function<void()>& f)
    {
        f();
        return !cancel;
    }

public:

    boost::thread thread;
    bool finish;
    bool ready;
    bool do_work;
    bool cancel;

    boost::mutex mutex;
    boost::condition_variable cond;

    boost::packaged_task<bool> task;
    boost::unique_future<bool> future;
};

render_thread_t::render_thread_t() : pimpl_( 0) {}
render_thread_t::~render_thread_t() { delete pimpl_;}

void render_thread_t::init()
{
    RAMEN_ASSERT( pimpl_ == 0 && "render_thread_t: init called twice");
    pimpl_ = new impl();
}

boost::unique_future<bool>& render_thread_t::render_image( node_renderer_t& renderer)
{
    return render_image( renderer, renderer.format());
}

boost::unique_future<bool>& render_thread_t::render_image(node_renderer_t& renderer,
                                                           const math::box2i_t& roi)
{
    return pimpl_->run_function( boost::bind( &node_renderer_t::render, boost::ref( renderer), roi));
}

bool render_thread_t::cancelled() const { return pimpl_->cancel;}

void render_thread_t::cancel_render() { pimpl_->cancel = true;}

} // render
} // ramen
