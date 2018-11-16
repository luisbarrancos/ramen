// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/imageio/png/png_format.hpp>

#include <ramen/imageio/oiio/oiio_reader.hpp>

#include <ramen/imageio/png/png_writer.hpp>

#include <ramen/imageio/factory.hpp>

namespace ramen
{
namespace imageio
{
png_format_t::png_format_t() {}
png_format_t::~png_format_t() {}

std::string png_format_t::tag() const { return std::string("png"); }

bool png_format_t::check_extension(const std::string& str) const
{
    return str == ".png" || str == ".PNG";
}

std::size_t png_format_t::detect_size() const { return 4; }

bool png_format_t::detect(const char* p) const
{
    const unsigned char* pp = reinterpret_cast<const unsigned char*>(p);
    return (pp[0] == 0x89) && (pp[1] == 0x50) && (pp[2] == 0x4E) &&
           (pp[3] == 0x47);
}

void png_format_t::add_extensions(std::vector<std::string>& ext) const
{
    ext.push_back("png");
    ext.push_back("PNG");
}

core::auto_ptr_t<reader_t> png_format_t::reader(
    const boost::filesystem::path& p) const
{
    return core::auto_ptr_t<reader_t>(new oiio_reader_t(p));
}

core::auto_ptr_t<writer_t> png_format_t::writer() const
{
    return core::auto_ptr_t<writer_t>(new png_writer_t());
}

static bool registered = factory_t::instance().register_image_format(
    core::auto_ptr_t<format_t>(new png_format_t()));

}  // namespace imageio
}  // namespace ramen
