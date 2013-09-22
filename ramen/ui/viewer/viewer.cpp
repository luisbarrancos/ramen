// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/viewer/viewer.hpp>

#include<ramen/assert.hpp>

namespace ramen
{
namespace ui
{

viewer_t::viewer_t( nodes::viewer_node_t *v)
{
    //RAMEN_ASSERT( v);

    viewer_node_ = v;
}

viewer_t::~viewer_t()
{
}

} // ui
} // ramen
