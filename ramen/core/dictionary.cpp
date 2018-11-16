// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include <ramen/core/dictionary.hpp>

#include <algorithm>

#include <boost/container/flat_map.hpp>

#include <ramen/core/exceptions.hpp>

namespace ramen
{
namespace core
{
struct dictionary_t::impl
{
    typedef boost::container::flat_map<key_type, value_type> map_type;
    typedef map_type::const_iterator                         const_iterator;
    typedef map_type::iterator                               iterator;

    map_type items;
};

dictionary_t::dictionary_t()
  : m_pimpl(0)
{
}

dictionary_t::~dictionary_t() { delete m_pimpl; }

// Copy constructor
dictionary_t::dictionary_t(const dictionary_t& other)
  : m_pimpl(0)
{
    if (other.m_pimpl)
        m_pimpl = new impl(*other.m_pimpl);
}

void dictionary_t::swap(dictionary_t& other)
{
    std::swap(m_pimpl, other.m_pimpl);
}

bool dictionary_t::empty() const
{
    if (m_pimpl)
        return m_pimpl->items.empty();

    return true;
}

dictionary_t::size_type dictionary_t::size() const
{
    if (m_pimpl)
        return m_pimpl->items.size();

    return 0;
}

void dictionary_t::clear()
{
    if (m_pimpl)
        m_pimpl->items.clear();
}

const dictionary_t::value_type& dictionary_t::operator[](
    const dictionary_t::key_type& key) const
{
    if (!m_pimpl)
        throw key_not_found(key);

    impl::const_iterator it(m_pimpl->items.find(key));

    if (it != m_pimpl->items.end())
        return it->second;
    else
        throw key_not_found(key);
}

dictionary_t::value_type& dictionary_t::operator[](
    const dictionary_t::key_type& key)
{
    if (!m_pimpl)
        m_pimpl = new impl();

    impl::iterator it(m_pimpl->items.find(key));

    if (it != m_pimpl->items.end())
        return it->second;
    else
    {
        std::pair<impl::iterator, bool> new_it(m_pimpl->items.insert(
            std::make_pair(key, dictionary_t::value_type())));
        return new_it.first->second;
    }
}

dictionary_t::const_iterator dictionary_t::begin() const
{
    if (m_pimpl)
        return reinterpret_cast<const_iterator>(
            m_pimpl->items.begin().get_ptr());

    return 0;
}

dictionary_t::const_iterator dictionary_t::end() const
{
    if (m_pimpl)
        return reinterpret_cast<const_iterator>(m_pimpl->items.end().get_ptr());

    return 0;
}

dictionary_t::iterator dictionary_t::begin()
{
    if (m_pimpl)
        return reinterpret_cast<iterator>(m_pimpl->items.begin().get_ptr());

    return 0;
}

dictionary_t::iterator dictionary_t::end()
{
    if (m_pimpl)
        return reinterpret_cast<iterator>(m_pimpl->items.end().get_ptr());

    return 0;
}

bool dictionary_t::operator==(const dictionary_t& other) const
{
    if (m_pimpl == 0 && other.m_pimpl == 0)
        return true;

    if (m_pimpl != 0 && other.m_pimpl != 0)
        return m_pimpl->items == other.m_pimpl->items;

    return false;
}

bool dictionary_t::operator!=(const dictionary_t& other) const
{
    return !(*this == other);
}

const dictionary_t::value_type* dictionary_t::get(
    const dictionary_t::key_type& key) const
{
    if (!m_pimpl)
        return 0;

    impl::const_iterator it(m_pimpl->items.find(key));

    if (it != m_pimpl->items.end())
        return &(it->second);

    return 0;
}

dictionary_t::value_type* dictionary_t::get(const dictionary_t::key_type& key)
{
    if (!m_pimpl)
        return 0;

    impl::iterator it(m_pimpl->items.find(key));

    if (it != m_pimpl->items.end())
        return &(it->second);

    return 0;
}

}  // namespace core
}  // namespace ramen
