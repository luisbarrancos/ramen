// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/config.hpp>

#include <ramen/core/memory.hpp>

namespace ramen
{
namespace memory
{
class pool_t
{
public:
    pool_t();
    ~pool_t();

    void init(std::size_t size_in_bytes);

    std::size_t pool_size() const;

    unsigned char* allocate(std::size_t& size);
    void           deallocate(unsigned char* p, std::size_t size);

private:
    // non-copyable
    pool_t(const pool_t&);
    pool_t& operator=(const pool_t&);

    // implementation is private...
    struct implementation_t;
    core::auto_ptr_t<implementation_t> m_pimpl;
    std::size_t                        allocated_;
};

}  // memory
}  // ramen

