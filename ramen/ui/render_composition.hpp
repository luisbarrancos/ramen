// Copyright (c) 2010 Esteban Tovagliari

#pragma once

#include <ramen/app/composition_fwd.hpp>

#include <functional>

namespace ramen
{
namespace ui
{
void render_composition(
    composition_t& comp,
    int            start,
    int            end,
    int            proxy_level,
    int            subsample,
    int            mb_extra_samples,
    float          mb_shutter_factor,
    bool           selected_only);

}  // namespace ui
}  // namespace ramen
