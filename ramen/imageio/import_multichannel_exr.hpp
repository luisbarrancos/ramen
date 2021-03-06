// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/filesystem/path.hpp>

namespace ramen
{
namespace imageio
{
void import_multichannel_exr(
    const boost::filesystem::path& p,
    bool                           relative,
    bool                           sequence);

}  // namespace imageio
}  // namespace ramen
