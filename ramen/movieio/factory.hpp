// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/config.hpp>

#include <vector>
#include <string>

#include <boost/ptr_container/ptr_vector.hpp>

#include <ramen/filesystem/path_sequence.hpp>

#include <ramen/movieio/format.hpp>
#include <ramen/movieio/exceptions.hpp>

namespace ramen
{
namespace movieio
{
class RAMEN_API factory_t
{
public:
    static factory_t& instance();

    ~factory_t();

    const boost::ptr_vector<format_t>& formats() const { return formats_; }

    typedef boost::ptr_vector<format_t>::const_iterator const_iterator;
    const_iterator                                      begin() const { return formats_.begin(); }
    const_iterator                                      end() const { return formats_.end(); }

    bool register_movie_format(core::auto_ptr_t<format_t> format);

    const std::vector<std::string>& extensions() const;

    core::auto_ptr_t<reader_t> create_reader(const boost::filesystem::path& p,
                                             bool                           sequence = true) const;
    core::auto_ptr_t<reader_t> create_reader(const filesystem::path_sequence_t& seq) const;

    core::auto_ptr_t<writer_t> writer_for_tag(const std::string& tag) const;

private:
    factory_t();

    // non-copyable
    factory_t(const factory_t&);
    factory_t& operator=(const factory_t&);

    const_iterator format_for_extension(const boost::filesystem::path& p) const;
    const_iterator format_for_file_contents(const boost::filesystem::path& p) const;
    const_iterator format_for_tag(const std::string& tag) const;

    boost::ptr_vector<format_t> formats_;

    mutable bool                     imageio_extensions_added_;
    mutable std::vector<std::string> extensions_;

    std::size_t   detect_size_;
    mutable char* detect_buffer_;
};

}  // movieio
}  // ramen

