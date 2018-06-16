// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_PALETTE_HPP
#define RAMEN_UI_PALETTE_HPP

#include <OpenEXR/ImathColor.h>

#include <QColor>

namespace ramen
{
namespace ui
{
class palette_t
{
public:
    static palette_t& instance();

    void rebuild();

    const Imath::Color3c& color(const std::string& k) const;
    QColor                qcolor(const std::string& k) const;

    bool find_color(const std::string& k) const;

    void set_color(const std::string& k, const Imath::Color3c& c);
    void set_color(const std::string& k, const QColor& c);

private:
    palette_t();
    ~palette_t();

    // Non-copyable.
    palette_t(const palette_t&) = delete;
    palette_t& operator=(const palette_t&) = delete;

    struct impl;
    impl* m_pimpl;
};

}  // namespace
}  // namespace

#endif
