// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/nodes/image_node.hpp>

#include <ramen/gil/extension/algorithm/tbb/tbb_copy.hpp>

namespace ramen
{
namespace image
{
namespace detail
{
template <class Fun> struct generate_pixels_fun
{
  public:
    generate_pixels_fun(
        const image::image_view_t& dst,
        const Imath::Box2i&        defined,
        Fun                        f)
      : dst_(dst)
      , f_(f)
    {
        defined_ = defined;
    }

    void operator()(const tbb::blocked_range<int>& r) const
    {
        for (int y = r.begin(); y < r.end(); ++y)
        {
            image::image_view_t::x_iterator dst_it(
                dst_.row_begin(y - defined_.min.y));

            for (int x = defined_.min.x; x <= defined_.max.x; ++x)
                *dst_it++ = f_(Imath::V2i(x, y));
        }
    }

  private:
    const image::image_view_t& dst_;
    Imath::Box2i               defined_;
    Fun                        f_;
};

}  // namespace detail

class RAMEN_API generator_node_t : public image_node_t
{
  public:
    generator_node_t();

    bool use_cache(const render::context_t& context) const override
    {
        return false;
    }

  protected:
    generator_node_t(const generator_node_t& other);
    void operator=(const generator_node_t&);

    void do_create_params() override;

    template <class Fun> void generate_pixels(Fun f)
    {
        detail::generate_pixels_fun<Fun> g(image_view(), defined(), f);
        tbb::parallel_for(
            tbb::blocked_range<int>(defined().min.y, defined().max.y + 1),
            g,
            tbb::auto_partitioner());
    }

  protected:
    void do_calc_format(const render::context_t& context) override;
    void do_calc_bounds(const render::context_t& context) override;
    void do_calc_defined(const render::context_t& context) override;

    void do_calc_hash_str(const render::context_t& context) override;
};

}  // namespace image
}  // namespace ramen
