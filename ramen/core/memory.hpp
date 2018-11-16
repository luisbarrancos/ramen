// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#include <ramen/config.hpp>

#include <stdint.h>

#include <boost/move/move.hpp>
#include <boost/static_assert.hpp>
#include <boost/swap.hpp>

#include <cassert>

namespace ramen
{
namespace core
{
template<class T>
struct pointer_traits;

template<class T>
struct pointer_traits<T*>
{
    typedef T                  element_type;
    typedef T*                 pointer_type;
    typedef const pointer_type const_pointer_type;

    static bool empty_ptr(T* x) { return x == 0; }
    static void delete_ptr(T* x) { delete x; }
};

template<class T>
struct pointer_traits<T (*)[]>
{
    typedef T                  element_type;
    typedef T*                 pointer_type;
    typedef const pointer_type const_pointer_type;

    static bool empty_ptr(T* x) { return x == 0; }
    static void delete_ptr(T* x) { delete[] x; }
};

/*!
\ingroup core
\brief An improved std::auto_ptr like smart pointer.
*/
template<class T>
class auto_ptr_t
{
    BOOST_MOVABLE_BUT_NOT_COPYABLE(auto_ptr_t)

    // for safe bool
    operator int() const;

public:
    typedef pointer_traits<T*>                       traits_type;
    typedef typename traits_type::element_type       element_type;
    typedef typename traits_type::pointer_type       pointer_type;
    typedef typename traits_type::const_pointer_type const_pointer_type;

    explicit auto_ptr_t(pointer_type ptr = 0)
    : ptr_(0)
    {
        reset(ptr);
    }

    ~auto_ptr_t() { delete_contents(); }

    auto_ptr_t(BOOST_RV_REF(auto_ptr_t) other)
    : ptr_(0)
    {
        reset();
        swap(other);
    }

    auto_ptr_t& operator=(BOOST_RV_REF(auto_ptr_t) other)
    {
        swap(other);
        return *this;
    }

    pointer_type get() const { return ptr_; }

    element_type& operator*() const
    {
        assert(this->get());

        return *this->get();
    }

    pointer_type operator->() const
    {
        assert(this->get());

        return this->get();
    }

    pointer_type release()
    {
        pointer_type rv = ptr_;
        ptr_            = 0;
        return rv;
    }

    void reset(pointer_type ptr = 0)
    {
        if (ptr_ != ptr)
        {
            delete_contents();
            ptr_ = ptr;
        }
    }

    void swap(auto_ptr_t& other) { boost::swap(ptr_, other.ptr_); }

    // safe bool conversion ( private int conversion prevents unsafe use)
    operator bool() const { return !traits_type::empty_ptr(this->get()); }

    bool operator!();

private:
    void delete_contents()
    {
        if (ptr_)
        {
            traits_type::delete_ptr(ptr_);
            ptr_ = 0;
        }
    }

    pointer_type ptr_;
};

template<class T>
inline void swap(auto_ptr_t<T>& x, auto_ptr_t<T>& y)
{
    x.swap(y);
}

template<class T>
T* aligned_ptr(T* p, int alignment)
{
    assert(((alignment - 1) & alignment) == 0);

    uintptr_t ptr     = reinterpret_cast<uintptr_t>(p);
    uintptr_t align   = alignment - 1;
    uintptr_t aligned = (ptr + align + 1) & ~align;

    return reinterpret_cast<unsigned char*>(aligned);
}

}  // core
}  // ramen

