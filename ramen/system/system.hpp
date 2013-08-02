// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_SYSTEM_HPP
#define RAMEN_SYSTEM_HPP

#include<ramen/system/system_fwd.hpp>

#include<string>

#include<boost/filesystem/path.hpp>

#include<ramen/app/application_fwd.hpp>

#include<ramen/os/system_info.hpp>

namespace ramen
{
namespace system
{

/**
\ingroup app
\brief class that contains os & hardware related info
*/
class RAMEN_API system_t
{
public:

    system_t();
    ~system_t();

    const core::string8_t& system_name() const
    {
        return system_name_;
    }

    const core::string8_t& user_name() const
    {
        return sys_info_.user_name();
    }

    const boost::filesystem::path& home_path() const
    {
        return sys_info_.home_path();
    }

    const boost::filesystem::path& executable_path() const
    {
        return sys_info_.executable_path();
    }

    const boost::filesystem::path& application_path() const
    {
        return application_path_;
    }

    const boost::filesystem::path& application_user_path() const
    {
        return application_user_path_;
    }

    // ram
    boost::uint64_t ram_size() const
    {
        return sys_info_.ram_size();
    }

private:

    // non-copyable
    system_t( const system_t&);
    system_t& operator=( const system_t&);

    core::string8_t system_name_;

    // paths
    boost::filesystem::path application_path_;
    boost::filesystem::path application_user_path_;

    struct impl;
    impl *pimpl_;

    os::system_info_t sys_info_;
};

} // system
} // ramen

#endif
