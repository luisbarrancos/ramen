// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

namespace ramen
{
namespace system
{

struct system_t::impl
{
    impl( system_t& self)
    {
        self.system_name_ = "OSX";

        // app user path
        {
            std::string dir_name( ".ramen");
            dir_name.append( RAMEN_VERSION_MAJOR_STR);
            self.application_user_path_ = self.home_path() / dir_name;
        }
    }
};

} // system
} // ramen
