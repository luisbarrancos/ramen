// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_ANIM_CURVES_VIEW_HPP
#define	RAMEN_ANIM_CURVES_VIEW_HPP

#include<QWidget>

#include<vector>

#include<boost/noncopyable.hpp>

#include<ramen/math/viewport.hpp>

#include<ramen/ui/anim/edit_keys_visitors.hpp>

class QAction;
class QPainter;

namespace ramen
{
namespace ui
{

class anim_curves_view_t : public QWidget, boost::noncopyable
{
    Q_OBJECT

public:

    anim_curves_view_t( QWidget *parent = 0);

    float time_scale() const;
    float value_scale() const;

    math::point2i_t world_to_screen( const math::point2f_t& p) const;
    math::point2f_t screen_to_world( const math::point2i_t& p) const;

    QPainter *painter() const { return painter_;}

    const math::viewport_t& viewport() const	{ return viewport_;}
    math::viewport_t& viewport()              { return viewport_;}

    bool snap_frames() const	{ return snap_;}
    bool show_tangents() const	{ return show_tangents_;}

    // draw / pick helpers
    int span_num_steps( float t0, float t1) const;
    void draw_small_box( const math::point2f_t& p) const;
    void draw_small_box( const math::point2i_t& p) const;

    float tangent_length() const;

    math::vector2f_t left_tangent_dir( float tangent, float yscale = 1.0f) const;
    math::vector2f_t right_tangent_dir( float tangent, float yscale = 1.0f) const;

    math::point2i_t left_tangent_pos( const math::point2i_t& p,
                                       float tangent,
                                       float yscale = 1.0f) const;

    math::point2i_t right_tangent_pos( const math::point2i_t& p,
                                        float tangent,
                                        float yscale = 1.0f) const;

    // draw & pick util
    int pick_distance() const;
    int pick_distance2() const;

    bool inside_pick_distance( const math::point2i_t& p, const math::point2i_t& q) const;

protected:

    virtual void keyPressEvent( QKeyEvent *event);
    virtual void keyReleaseEvent( QKeyEvent *event);
    virtual void mouseMoveEvent( QMouseEvent *event);
    virtual void mousePressEvent( QMouseEvent *event);
    virtual void mouseReleaseEvent( QMouseEvent *event);
    virtual void paintEvent ( QPaintEvent *event);
    virtual void resizeEvent( QResizeEvent *event);
    virtual void contextMenuEvent( QContextMenuEvent *event);

public Q_SLOTS:

    void frame_all();
    void frame_selection();

private Q_SLOTS:

    void set_snap_frames( bool b);
    void set_show_tangents( bool b);

private:

    void reset_view();

    void draw_grid() const;
    void draw_axes() const;
    void draw_time_bar() const;

    // from paul heckberts' code
    double nice_num( double x, bool round) const;

    int abs( int x) const
    {
        if( x < 0)
            return -x;

        return x;
    }

    void frame_area( const math::box2f_t& area);

    QAction *frame_all_, *frame_selected_;
    QAction *snap_frames_, *show_tans_;

    bool first_resize_;
    math::viewport_t viewport_;

    int push_x_, push_y_;
    int last_x_, last_y_;
    bool handle_mouse_event_;

    bool scroll_mode_;
    bool zoom_mode_;
    math::point2f_t zoom_center_;

    bool move_time_mode_;
    bool snap_;
    bool show_tangents_;

    QPainter *painter_;
};

} // ui
} // ramen

#endif
