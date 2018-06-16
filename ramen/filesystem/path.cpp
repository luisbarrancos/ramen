// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/filesystem/path.hpp>

#include <sstream>

#include <boost/filesystem/operations.hpp>

#include <QDir>

#include <cassert>

namespace ramen
{
namespace filesystem
{
const std::string& file_string(const boost::filesystem::path& p) { return p.string(); }
const char*        file_cstring(const boost::filesystem::path& p) { return p.string().c_str(); }

std::string hash_string(const boost::filesystem::path& p)
{
    std::stringstream s;
    s << p;

    if (boost::filesystem::exists(p))
    {
        std::time_t t = boost::filesystem::last_write_time(p);
        s << t;
    }

    return s.str();
}

boost::filesystem::path make_absolute_path(const boost::filesystem::path& p,
                                           const boost::filesystem::path& from)
{
    assert(p.is_relative());
    assert(from.is_absolute());

    QDir    dir(QString(file_cstring(from)));
    QString fname(QString(file_cstring(p)));
    QString abs_path(QDir::cleanPath(dir.absoluteFilePath(fname)));
    return boost::filesystem::path(abs_path.toStdString());
}

boost::filesystem::path make_relative_path(const boost::filesystem::path& p,
                                           const boost::filesystem::path& from)
{
    assert(p.is_absolute());
    assert(from.is_absolute());

    QDir    dir(QString(file_cstring(from)));
    QString fname(QString(file_cstring(p)));
    QString rel_path(dir.relativeFilePath(fname));
    return boost::filesystem::path(rel_path.toStdString());
}

boost::filesystem::path convert_relative_path(const boost::filesystem::path& p,
                                              const boost::filesystem::path& old_base,
                                              const boost::filesystem::path& new_base)
{
    boost::filesystem::path p0(make_absolute_path(p, old_base));
    return make_relative_path(p0, new_base);
}

}  // filesystem
}  // ramen
