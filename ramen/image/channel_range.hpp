// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

namespace ramen
{
namespace image
{
class channel_range_t
{
  public:
    channel_range_t() {}

    float operator()(float x) const;

    float center;
    float tol_lo, tol_hi;
    float soft_lo, soft_hi;
};

}  // namespace image
}  // namespace ramen
