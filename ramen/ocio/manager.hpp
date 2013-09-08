// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_OCIO_MANAGER_HPP
#define	RAMEN_OCIO_MANAGER_HPP

#include<ramen/ocio/manager_fwd.hpp>

#include<vector>

#include<boost/filesystem/path.hpp>

#include<OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;

#include<ramen/core/string8.hpp>

namespace ramen
{
namespace ocio
{

class RAMEN_API manager_t
{
public:

    manager_t();
    ~manager_t();

    OCIO::ConstConfigRcPtr config() const;

    const std::vector<core::string8_t>& displays() const
    {
        return displays_;
    }

    const core::string8_t& default_display() const
    {
        return displays()[default_display_index()];
    }

    int default_display_index() const
    {
        return default_display_index_;
    }

    void get_views( const core::string8_t& display,
                    std::vector<core::string8_t>& views,
                    int& default_index) const;

    core::string8_t default_view( const core::string8_t& display) const;

private:

    // noncopyable
    manager_t( const manager_t&);
    manager_t& operator=( const manager_t&);

    void init();
    bool init_from_file( const boost::filesystem::path& p);

    void get_displays();

    core::string8_t default_display_;
    int default_display_index_;
    std::vector<core::string8_t> displays_;
};

} // ocio
} // ramen

#endif
