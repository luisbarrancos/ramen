// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_PREFERENCES_HPP
#define RAMEN_PREFERENCES_HPP

#include<vector>
#include<string>

#include<boost/noncopyable.hpp>

#include<ramen/app/application_fwd.hpp>

namespace ramen
{

/**
\ingroup app
\brief contains the user preferences
*/
class preferences_t : boost::noncopyable
{
public:

    // memory
    int max_image_memory() const        { return max_image_memory_;}
    void set_max_image_memory( int m)   { max_image_memory_ = m;}

    int frame_rate() const      { return frame_rate_;}
    void set_frame_rate( int f) { frame_rate_ = f;}

    // image limits
    int max_image_width() const;
    int max_image_height() const;

    // user interface
    int pick_distance() const { return pick_distance_;}

    void save();

private:

    friend class application_t;

    preferences_t();

    void set_defaults();

    void load();

    int frame_rate_;
    int max_image_memory_;
    int pick_distance_;
};

} // ramen

#endif
