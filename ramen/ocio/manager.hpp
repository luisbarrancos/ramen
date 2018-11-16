// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/ocio/manager_fwd.hpp>

#include <vector>
#include <string>

#include <boost/filesystem/path.hpp>

#include <OpenColorIO/OpenColorIO.h>
namespace OCIO = OCIO_NAMESPACE;

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

    const std::vector<std::string>& displays() const { return displays_; }

    const std::string& default_display() const
    {
        return displays()[default_display_index()];
    }

    int default_display_index() const { return default_display_index_; }

    void get_views(
        const std::string&        display,
        std::vector<std::string>& views,
        int&                      default_index) const;
    std::string default_view(const std::string& display) const;

  private:
    // noncopyable
    manager_t(const manager_t&);
    manager_t& operator=(const manager_t&);

    void init();
    bool init_from_file(const boost::filesystem::path& p);

    void get_displays();

    std::string              default_display_;
    int                      default_display_index_;
    std::vector<std::string> displays_;
};

}  // namespace ocio
}  // namespace ramen
