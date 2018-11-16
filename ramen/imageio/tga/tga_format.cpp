// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/imageio/tga/tga_format.hpp>

#include <ramen/imageio/oiio/oiio_reader.hpp>

#include <ramen/imageio/tga/tga_writer.hpp>

#include <ramen/imageio/factory.hpp>

namespace ramen
{
namespace imageio
{
tga_format_t::tga_format_t() {}
tga_format_t::~tga_format_t() {}

std::string tga_format_t::tag() const { return std::string("tga"); }

bool tga_format_t::check_extension(const std::string& str) const
{
    return str == ".tga" || str == ".TGA";
}

std::size_t tga_format_t::detect_size() const
{
    // return 18;
    return 0;
}

bool tga_format_t::detect(const char* p) const
{
    // we can do better
    return false;
}

void tga_format_t::add_extensions(std::vector<std::string>& ext) const
{
    ext.push_back("tga");
    ext.push_back("TGA");
}

core::auto_ptr_t<reader_t> tga_format_t::reader(
    const boost::filesystem::path& p) const
{
    return core::auto_ptr_t<reader_t>(new oiio_reader_t(p));
}

core::auto_ptr_t<writer_t> tga_format_t::writer() const
{
    return core::auto_ptr_t<writer_t>(new tga_writer_t());
}

static bool registered = factory_t::instance().register_image_format(
    core::auto_ptr_t<format_t>(new tga_format_t()));

}  // namespace imageio
}  // namespace ramen
