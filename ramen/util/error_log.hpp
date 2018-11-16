// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <sstream>

namespace ramen
{
namespace util
{
class error_log_t
{
public:
    error_log_t();

    template<class T>
    error_log_t& operator<<(const T& x)
    {
        error_stream() << x;
        return *this;
    }

    std::stringstream& error_stream();
    std::string        errors() const;

private:
    std::stringstream error_stream_;
};

}  // util
}  // ramen

