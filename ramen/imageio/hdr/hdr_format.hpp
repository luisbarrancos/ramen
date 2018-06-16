// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_HDR_FORMAT_HPP
#define RAMEN_HDR_FORMAT_HPP

#include <ramen/imageio/format.hpp>

namespace ramen
{
namespace imageio
{
class hdr_format_t : public format_t
{
public:
    hdr_format_t();
    ~hdr_format_t() override;

    virtual std::string tag() const;

    virtual bool check_extension(const std::string& str) const;

    virtual std::size_t detect_size() const;
    bool        detect(const char* p) const override;

    virtual void add_extensions(std::vector<std::string>& ext) const;

    bool is_multichannel() const override { return false; }

    core::auto_ptr_t<reader_t> reader(const boost::filesystem::path& p) const override;
    core::auto_ptr_t<writer_t> writer() const override;
};

}  // imageio
}  // ramen

#endif
