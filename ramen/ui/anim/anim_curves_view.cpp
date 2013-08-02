// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/anim/anim_curves_view.hpp>

#include<algorithm>

#include<boost/foreach.hpp>
#include<boost/bind.hpp>

#include<QTreeView>
#include<QKeyEvent>
#include<QMouseEvent>
#include<QPaintEvent>
#include<QResizeEvent>
#include<QPainter>
#include<QPen>
#include<QBrush>
#include<QAction>
#include<QContextMenuEvent>
#include<QMenu>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/math/cmath.hpp>

#include<ramen/algorithm/clamp.hpp>

#include<ramen/iterators/nice_numbers.hpp>

#include<ramen/anim/track.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/anim/anim_editor.hpp>
#include<ramen/ui/anim/anim_editor_toolbar.hpp>
#include<ramen/ui/anim/draw_curves_visitors.hpp>
#include<ramen/ui/palette.hpp>

#include<iostream>

namespace ramen
{
namespace ui
{

anim_curves_view_t::anim_curves_view_t( QWidget *parent) : QWidget( parent), first_resize_( true)
{
    setFocusPolicy( Qt::ClickFocus);
    viewport_.set_y_down( true);
    scroll_mode_ = false;
    handle_mouse_event_ = false;
    snap_ = false;
    show_tangents_ = true;

    frame_selected_ = new QAction( "Frame Selected", this);
    connect( frame_selected_, SIGNAL( triggered()), this, SLOT( frame_selection()));

    frame_all_ = new QAction( "Frame All", this);
    connect( frame_all_, SIGNAL( triggered()), this, SLOT( frame_all()));

    snap_frames_ = new QAction( "Snap Frames", this);
    snap_frames_->setCheckable( true);
    snap_frames_->setChecked( false);
    connect( snap_frames_, SIGNAL( triggered( bool)), this, SLOT( set_snap_frames( bool)));

    show_tans_ = new QAction( "Show Tangents", this);
    show_tans_->setCheckable( true);
    show_tans_->setChecked( true);
    connect( show_tans_, SIGNAL( triggered( bool)), this, SLOT( set_show_tangents( bool)));
}

float anim_curves_view_t::time_scale() const	{ return viewport_.zoom_x();}
float anim_curves_view_t::value_scale() const	{ return viewport_.zoom_y();}

math::point2i_t anim_curves_view_t::world_to_screen( const math::point2f_t& p) const
{
    return viewport_.world_to_screen( p);
}

math::point2f_t anim_curves_view_t::screen_to_world( const math::point2i_t& p) const
{
    return viewport_.screen_to_world( p);
}

void anim_curves_view_t::keyPressEvent( QKeyEvent *event)
{
    switch( event->key())
    {
    case Qt::Key_Home:
        reset_view();
        update();
        event->accept();
    break;

    case Qt::Key_Comma:
    {
        math::point2f_t p( viewport_.screen_to_world( viewport_.device().center()));
        viewport_.zoom( p, 1.33f);
        update();
        event->accept();
    }
    break;

    case Qt::Key_Period:
    {
        math::point2f_t p( viewport_.screen_to_world( viewport_.device().center()));
        viewport_.zoom( p, 0.66f);
        update();
        event->accept();
    }
    break;

    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        app().ui()->anim_editor().delete_selected_keyframes();
        event->accept();
    break;

    default:
        app().ui()->anim_editor().toolbar().tool()->key_press_event( *this, event);
    break;
    }
}

void anim_curves_view_t::keyReleaseEvent( QKeyEvent *event)
{
    switch( event->key())
    {
    case Qt::Key_Home:
    case Qt::Key_Comma:
    case Qt::Key_Period:
    case Qt::Key_Backspace:
    case Qt::Key_Delete:
        event->accept();
    break;

    default:
        app().ui()->anim_editor().toolbar().tool()->key_release_event( *this, event);
    break;
    }
}

void anim_curves_view_t::mousePressEvent( QMouseEvent *event)
{
    if( event->button() != Qt::LeftButton)
    {
        handle_mouse_event_ = false;
        QWidget::mousePressEvent( event);
        return;
    }

    handle_mouse_event_ = true;
    push_x_ = event->x();
    push_y_ = event->y();
    last_x_ = event->x();
    last_y_ = event->y();

    scroll_mode_ = false;
    zoom_mode_ = false;
    zoom_center_ = screen_to_world( math::point2i_t( push_x_, push_y_));
    move_time_mode_ = false;

    if( event->modifiers() & Qt::AltModifier)
    {
        if( event->modifiers() & Qt::ShiftModifier)
        {
            zoom_mode_ = true;
            zoom_center_ = screen_to_world( math::point2i_t( push_x_, push_y_));
        }
        else
            scroll_mode_ = true;

        event->accept();
        return;
    }

    float time = app().document().composition_node().frame();
    math::point2i_t q( world_to_screen( math::point2f_t( time, 0)));

    if( abs( q.x - push_x_) <= 4)
    {
        move_time_mode_ = true;
        event->accept();
    }
    else
    {
        app().ui()->begin_interaction();
        app().ui()->anim_editor().toolbar().tool()->mouse_press_event( *this, event);
    }
}

void anim_curves_view_t::mouseMoveEvent( QMouseEvent *event)
{
    if( !handle_mouse_event_)
    {
        QWidget::mouseMoveEvent( event);
        return;
    }

    if( ( event->x() != last_x_) || ( event->y() != last_y_))
    {
        if( scroll_mode_)
        {
            viewport_.scroll( math::vector2i_t( -(event->x() - last_x_), -(event->y() - last_y_)));
            update();
            event->accept();
        }
        else
        {
            if( zoom_mode_)
            {
                const float zoom_speed = 0.05f;
                float zoomx = 1.0f + ( zoom_speed * ( event->x() - last_x_));
                float zoomy = 1.0f + ( zoom_speed * ( event->y() - last_y_));

                viewport_.zoom( zoom_center_, zoomx, zoomy);
                update();
                event->accept();
            }
            else
            {
                if( move_time_mode_)
                {
                    math::point2i_t p( event->x(), event->y());
                    math::point2f_t q( screen_to_world( p));

                    if( q.x < app().ui()->start_frame())
                        q.x = app().ui()->start_frame();

                    if( q.x > app().ui()->end_frame())
                        q.x = app().ui()->end_frame();

                    if( q.x != app().ui()->frame())
                        app().ui()->set_frame( q.x);
                }
                else
                    app().ui()->anim_editor().toolbar().tool()->mouse_drag_event( *this, event);
            }
        }

        last_x_ = event->x();
        last_y_ = event->y();
    }
}

void anim_curves_view_t::mouseReleaseEvent( QMouseEvent *event)
{
    if( !handle_mouse_event_)
    {
        QWidget::mouseReleaseEvent( event);
        return;
    }

    if( !scroll_mode_ && !zoom_mode_ && !move_time_mode_)
    {
        app().ui()->anim_editor().toolbar().tool()->mouse_release_event( *this, event);
        app().ui()->end_interaction();
    }
    else
        event->accept();

    scroll_mode_ = false;
    zoom_mode_ = false;
    move_time_mode_ = false;
}

void anim_curves_view_t::paintEvent( QPaintEvent *event)
{
    QPainter painter( this);
    painter_ = &painter; // save the ptr for use in members
    painter.setRenderHint( QPainter::Antialiasing);
    painter.setPen( Qt::NoPen);

    // draw bg
    QBrush brush;
    brush.setColor( palette_t::instance().qcolor( "background"));
    brush.setStyle( Qt::SolidPattern);
    painter.setBrush( brush);
    painter.drawRect( 0, 0, width(), height());

    draw_grid();

    // draw in world space
    math::matrix33f_t view_xform = viewport_.world_to_screen_matrix();
    QMatrix qm( view_xform( 0, 0), view_xform( 0, 1),
                view_xform( 1, 0), view_xform( 1, 1),
                view_xform( 2, 0), view_xform( 2, 1));
    painter.setWorldTransform( QTransform( qm));

    QPen pen;

    // draw curves
    draw_curve_visitor v( *this);

    BOOST_FOREACH( const anim::track_t *t, app().ui()->anim_editor().active_tracks())
    {
        pen.setColor( QColor( t->color().x, t->color().y, t->color().z));
        painter.setPen( pen);
        boost::apply_visitor( v, t->curve().get());
    }

    // draw in screen space
    painter.setWorldTransform( QTransform());

    pen.setColor( palette_t::instance().qcolor( "text"));
    painter.setPen( pen);

    draw_keyframes_visitor v2( *this, show_tangents());
    BOOST_FOREACH( const anim::track_t *t, app().ui()->anim_editor().active_tracks())
        boost::apply_visitor( v2, t->curve().get());

    draw_axes();
    draw_time_bar();

    app().ui()->anim_editor().toolbar().tool()->draw_overlay( *this);

    event->accept();
    painter_ = 0;
}

void anim_curves_view_t::resizeEvent( QResizeEvent *event)
{
    if( first_resize_)
    {
        reset_view();
        first_resize_ = false;
        return;
    }

    viewport_.resize( event->size().width(), event->size().height());
    event->accept();
}

void anim_curves_view_t::contextMenuEvent( QContextMenuEvent *event)
{
    QMenu menu( this);
    menu.addAction( frame_selected_);
    menu.addAction( frame_all_);
    menu.addSeparator();

    snap_frames_->setChecked( snap_);
    menu.addAction( snap_frames_);
    menu.addAction( show_tans_);

    menu.exec(event->globalPos());
    event->accept();
}

void anim_curves_view_t::reset_view()
{
    int start_frame = app().document().composition_node().start_frame();
    int end_frame = app().document().composition_node().end_frame();

    viewport_.reset( math::box2i_t( math::point2i_t( 0, 0), math::point2i_t( width() - 1, height() - 1)),
                     math::box2f_t( math::point2f_t( start_frame - 5, 0),
                                    math::point2f_t( end_frame + 5, 10)));
}

void anim_curves_view_t::draw_grid() const
{
    const int spacing = 30;

    QPen pen;
    pen.setColor( palette_t::instance().qcolor( "grid"));
    pen.setWidth( 1);
    painter_->setPen( pen);

    // vertical
    {
        int nticks = math::cmath<double>::floor( (double) width() / spacing);
        for( iterators::nice_numbers_t it( viewport_.world().min.x, viewport_.world().max.x, nticks), e; it != e; ++it)
        {
            math::point2f_t p( *it, 0);
            math::point2i_t q( world_to_screen( p));
            painter_->drawLine( QPointF( q.x, 0), QPointF( q.x, height()));
        }
    }

    // horizontal
    {
        int nticks = math::cmath<double>::floor( (double) height() / spacing);
        for( iterators::nice_numbers_t it( viewport_.world().min.y, viewport_.world().max.y, nticks), e; it != e; ++it)
        {
            math::point2f_t p( 0, *it);
            math::point2i_t q( world_to_screen( p));
            painter_->drawLine( QPointF( 0, q.y), QPointF( width(), q.y));
        }
    }
}

void anim_curves_view_t::draw_axes() const
{
    const int spacing = 30;

    QPen pen;
    pen.setColor( QColor( 0, 0, 0));
    pen.setWidth( 1);
    painter_->setPen( pen);

    // horizontal
    {
        painter_->drawLine( QPointF( 0, height() - spacing), QPointF( width(), height() - spacing));
        int nticks = math::cmath<double>::floor( (double) width() / spacing);
        for( iterators::nice_numbers_t it( viewport_.world().min.x, viewport_.world().max.x, nticks), e; it != e; ++it)
        {
            math::point2f_t p( *it, 0);
            math::point2i_t q( world_to_screen( p));
            painter_->drawLine( QPointF( q.x, height() - spacing - 3), QPointF( q.x, height() - spacing + 3));
            painter_->drawText( QPoint( q.x, height() - 10), QString::number( *it));
        }
    }

    // vertical
    {
        painter_->drawLine( QPointF( spacing * 2, 0), QPointF( spacing * 2, height()));
        int nticks = math::cmath<double>::floor( (double) height() / spacing);
        for( iterators::nice_numbers_t it( viewport_.world().min.y, viewport_.world().max.y, nticks), e; it != e; ++it)
        {
            math::point2f_t p( 0, *it);
            math::point2i_t q( world_to_screen( p));
            painter_->drawLine( QPointF( spacing * 2 - 3, q.y), QPointF( spacing * 2 + 3, q.y));
            painter_->drawText( QPointF( 20, q.y), QString::number( *it));
        }
    }
}

void anim_curves_view_t::draw_time_bar() const
{
    float time = app().document().composition_node().frame();
    math::point2i_t q( world_to_screen( math::point2f_t( time, 0)));

    QPen pen;
    pen.setColor( QColor( 255, 0, 0));
    pen.setWidth( 2);
    painter_->setPen( pen);
    painter_->drawLine( QPoint( q.x, 0), QPoint( q.x, height()));
}

void anim_curves_view_t::frame_area( const math::box2f_t& area)
{
    if( area.is_empty())
        return;

    math::box2f_t new_area( area);
    new_area.min.x -= area.size().x / 10;
    new_area.max.x += area.size().x / 10;
    new_area.min.y -= area.size().y / 10;
    new_area.max.y += area.size().y / 10;

    if( new_area.size().x == 0)
    {
        float half_size = viewport_.world().size().x / 2;
        new_area.min.x -= half_size;
        new_area.max.x += half_size;
    }

    if( new_area.size().y == 0)
    {
        float half_size = viewport_.world().size().y / 2;
        new_area.min.y -= half_size;
        new_area.max.y += half_size;
    }

    viewport_.reset( viewport_.device(), new_area);
}

// draw / pick helpers

int anim_curves_view_t::span_num_steps( float t0, float t1) const
{
    float pixel_len = ( t1 - t0) * time_scale();
    return algorithm::clamp( (int) pixel_len / 5, 5, 50);
}

void anim_curves_view_t::draw_small_box( const math::point2f_t& p) const
{
    painter()->drawRect( QRectF( p.x - 3, p.y - 3, 5, 5));
}

void anim_curves_view_t::draw_small_box( const math::point2i_t& p) const
{
    painter()->drawRect( QRect( p.x - 3, p.y - 3, 5, 5));
}

float anim_curves_view_t::tangent_length() const { return 20.0f;}

math::vector2f_t anim_curves_view_t::left_tangent_dir( float tangent, float yscale) const
{
    math::vector2f_t tv( -time_scale(), tangent * value_scale() * yscale);
    tv.normalize();
    return tv;
}

math::vector2f_t anim_curves_view_t::right_tangent_dir( float tangent, float yscale) const
{
    math::vector2f_t tv( time_scale(), -tangent * value_scale() * yscale);
    tv.normalize();
    return tv;
}

math::point2i_t anim_curves_view_t::left_tangent_pos( const math::point2i_t& p,
                                                      float tangent,
                                                      float yscale) const
{
    math::vector2f_t tv( left_tangent_dir( tangent, yscale));
    tv *= tangent_length();
    return math::point2i_t( p.x + tv.x, p.y + tv.y);
}

math::point2i_t anim_curves_view_t::right_tangent_pos( const math::point2i_t& p,
                                                       float tangent,
                                                       float yscale) const
{
    math::vector2f_t tv( right_tangent_dir( tangent, yscale));
    tv *= tangent_length();
    return math::point2i_t( p.x + tv.x, p.y + tv.y);
}

int anim_curves_view_t::pick_distance() const	{ return 5;}
int anim_curves_view_t::pick_distance2() const	{ return pick_distance() * pick_distance();}

bool anim_curves_view_t::inside_pick_distance( const math::point2i_t& p,
                                               const math::point2i_t& q) const
{
    int l = math::squared_distance( p,q);
    int m = pick_distance2();
    return l < m;
}

// slots
void anim_curves_view_t::frame_all()
{
    bbox_curve_visitor v;

    BOOST_FOREACH( const anim::track_t *t, app().ui()->anim_editor().active_tracks())
        boost::apply_visitor( v, t->curve().get());

    frame_area( v.bbox);
}

void anim_curves_view_t::frame_selection()
{
    bbox_curve_visitor v( math::box2f_t(), true);

    BOOST_FOREACH( const anim::track_t *t, app().ui()->anim_editor().active_tracks())
        boost::apply_visitor( v, t->curve().get());

    frame_area( v.bbox);
}

void anim_curves_view_t::set_snap_frames( bool b) { snap_ = b;}

void anim_curves_view_t::set_show_tangents( bool b)
{
    show_tangents_ = b;
    update();
}

} // namespace
} // namespace
