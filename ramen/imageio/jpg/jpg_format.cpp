// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/imageio/jpg/jpg_format.hpp>
#include <ramen/imageio/jpg/jpg_reader.hpp>
#include <ramen/imageio/jpg/jpg_writer.hpp>

#include <ramen/imageio/factory.hpp>

namespace ramen
{
namespace imageio
{
jpg_format_t::jpg_format_t() {}

std::string jpg_format_t::tag() const { return std::string("jpg"); }

bool jpg_format_t::check_extension(const std::string& str) const
{
    return str == ".jpg" || str == ".JPG";
}

std::size_t jpg_format_t::detect_size() const { return 4; }

bool jpg_format_t::detect(const char* p) const
{
    const unsigned char* pp = reinterpret_cast<const unsigned char*>(p);
    return (pp[0] == 0xFF) && (pp[1] == 0xD8) && (pp[2] == 0xFF) &&
           (pp[3] == 0xE0);
}

void jpg_format_t::add_extensions(std::vector<std::string>& ext) const
{
    ext.push_back("jpg");
    ext.push_back("JPG");
}

core::auto_ptr_t<reader_t> jpg_format_t::reader(
    const boost::filesystem::path& p) const
{
    return core::auto_ptr_t<reader_t>(new jpg_reader_t(p));
}

core::auto_ptr_t<writer_t> jpg_format_t::writer() const
{
    return core::auto_ptr_t<writer_t>(new jpg_writer_t());
}

static bool registered = factory_t::instance().register_image_format(
    core::auto_ptr_t<format_t>(new jpg_format_t()));

}  // namespace imageio
}  // namespace ramen
