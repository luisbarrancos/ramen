// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/nodes/node_info.hpp>

#include<ramen/nodes/node.hpp>

namespace ramen
{

node_info_t::node_info_t()
{
    ui_visible = true;
    help = 0;
    first_time_ = true;
}

} // namespace
