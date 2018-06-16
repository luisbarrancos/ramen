// Copyright (c) 2010 Esteban Tovagliari

#include <ramen/render/render_thread.hpp>

#include <cassert>
#include <thread>

#include <boost/bind.hpp>

namespace ramen
{
namespace render
{
struct render_thread_t::impl
{
    impl()
    {
        // start our thread
        finish  = false;
        ready   = false;
        do_work = false;
        thread  = std::thread(&impl::thread_loop, this);
    }

    ~impl()
    {
        boost::unique_lock<boost::mutex> lock(mutex);
        finish  = true;
        ready   = true;
        do_work = false;
        lock.unlock();
        cond.notify_one();

        // wait for our thread to exit
        thread.join();
    }

    boost::unique_future<bool>& run_function(const boost::function<void()>& f)
    {
        assert(!do_work && "render_thread_t is not reentrant");
        boost::unique_lock<boost::mutex> lock(mutex);
        cancel  = false;
        ready   = true;
        do_work = true;
        task    = boost::packaged_task<bool>(boost::bind(&impl::call_fun, this, f));
        future  = task.get_future();
        lock.unlock();
        cond.notify_one();
        return future;
    }

private:
    void thread_loop()
    {
        while (1)
        {
            boost::unique_lock<boost::mutex> lock(mutex);

            while (!ready)
                cond.wait(lock);

            if (finish)
                break;

            if (do_work)
            {
                do_work = false;
                ready   = false;
                task();
            }
        }
    }

    bool call_fun(const boost::function<void()>& f)
    {
        f();
        return !cancel;
    }

public:
    std::thread   thread;
    bool          finish;
    bool          ready;
    bool          do_work;
    bool          cancel;

    boost::mutex              mutex;
    boost::condition_variable cond;

    boost::packaged_task<bool> task;
    boost::unique_future<bool> future;
};

render_thread_t::render_thread_t()
: m_pimpl(0)
{
}
render_thread_t::~render_thread_t() { delete m_pimpl; }

void render_thread_t::init()
{
    assert(m_pimpl == 0 && "render_thread_t: init called twice");
    m_pimpl = new impl();
}

boost::unique_future<bool>& render_thread_t::render_image(image_node_renderer_t& renderer)
{
    return render_image(renderer, renderer.format());
}

boost::unique_future<bool>& render_thread_t::render_image(image_node_renderer_t& renderer,
                                                          const Imath::Box2i&    roi)
{
    return m_pimpl->run_function(
        boost::bind(&image_node_renderer_t::render, boost::ref(renderer), roi));
}

bool render_thread_t::cancelled() const { return m_pimpl->cancel; }

void render_thread_t::cancel_render() { m_pimpl->cancel = true; }

}  // namespace
}  // namespace
