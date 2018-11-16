// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/ui/viewer/image_view/image_strategy.hpp>

#include <boost/noncopyable.hpp>
#include <boost/ptr_container/ptr_vector.hpp>

#include <memory>
#include <vector>

namespace ramen
{
namespace ui
{
namespace viewer
{
class tiled_image_strategy_t : public image_strategy_t
{
public:
    tiled_image_strategy_t(const image::buffer_t& pixels,
                           const Imath::Box2i&    display_window,
                           const Imath::Box2i&    data_window,
                           GLenum                 texture_unit = GL_TEXTURE0);

    bool update_pixels(const image::buffer_t& pixels,
                       const Imath::Box2i&    display_window,
                       const Imath::Box2i&    data_window) override;

    void draw() const override;

    boost::optional<Imath::Color4f> color_at(const Imath::V2i& p) const override;

private:
    static int tile_size();

    Imath::Box2i area_for_tile(int x, int y) const;

    struct tile_t : boost::noncopyable
    {
    public:
        tile_t(const image::buffer_t& pixels, const Imath::Box2i& area);
        ~tile_t();

        void update_texture(const Imath::Box2i& area, char* ptr, std::size_t rowsize);

        void draw() const;

        static char*       pixel_ptr(const image::buffer_t& pixels, int x, int y);
        static std::size_t rowbytes(const image::buffer_t& pixels);

    private:
        void alloc_tile(int width, int height);

        GLuint       texture_id_;
        Imath::Box2i area_;
    };

    image::buffer_t                      pixels_;
    std::vector<std::unique_ptr<tile_t>> tiles_;
};

}  // viewer
}  // ui
}  // ramen

