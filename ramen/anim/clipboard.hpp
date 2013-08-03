// Copyright (c) 2011 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_ANIM_CLIPBOARD_HPP
#define	RAMEN_ANIM_CLIPBOARD_HPP

#include<ramen/config.hpp>

#include<vector>
#include<memory>

#include<boost/shared_ptr.hpp>

#include<ramen/core/string8.hpp>

#include<ramen/string_algo/edit_distance.hpp>

#include<ramen/anim/any_curve.hpp>

namespace ramen
{
namespace anim
{

class clipboard_t
{
public:

    typedef std::pair<core::string8_t, boost::shared_ptr<any_curve_t> > named_curve_type;

    static clipboard_t& instance();

    bool empty() const;
    void clear();

    void begin_copy();

    void copy_curve( const core::string8_t& name, const anim::any_curve_ptr_t& c);
    void copy_keys( const core::string8_t& name, const anim::any_curve_ptr_t& c);

    void end_copy();

    bool can_paste( const core::string8_t& name, const anim::any_curve_ptr_t& c);
    void paste( const core::string8_t& name, anim::any_curve_ptr_t& c, float frame);

    // param spinboxes
    bool can_paste();
    void copy( const float_curve_t& c);
    void paste( float_curve_t& dst);

private:

    clipboard_t();
    ~clipboard_t();

    // non-copyable
    clipboard_t( const clipboard_t&);
    clipboard_t& operator=( const clipboard_t&);

    int find_compatible_curve( const core::string8_t& name, const anim::any_curve_ptr_t& c);

    std::vector<named_curve_type> contents_;
    bool copy_curves_mode_;
    bool copying_;

    string_algo::edit_distance_t<core::string8_t> distance_fun_;
};

} // anim
} // ramen

#endif
