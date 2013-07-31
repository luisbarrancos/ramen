// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/palette.hpp>

#include<QApplication>
#include<QPalette>

#include<ramen/assert.hpp>

#include<iostream>

namespace ramen
{
namespace ui
{

struct palette_t::impl
{
    impl() { rebuild_palette();}

    void rebuild_palette()
    {
        QColor bgcol = QApplication::palette().color( QPalette::Base);
        QColor fgcol = QApplication::palette().color( QPalette::Text);

        error_color_ = color::color3c_t( 255, 0, 255);

        colors_["background"]   = color::color3c_t( bgcol.red(), bgcol.green(), bgcol.blue());
        colors_["text"]   = color::color3c_t( fgcol.red(), fgcol.green(), fgcol.blue());

        colors_["front plug"]   = color::color3c_t( 255, 0, 0);
        colors_["back plug"]    = color::color3c_t( 0, 255, 0);
        colors_["matte plug"]   = color::color3c_t( 0, 0, 255);
        colors_["out plug"]	    = color::color3c_t( 255, 255, 0);
        colors_["optional plug"] = color::color3c_t( 0, 255, 0);

        colors_["node_shadow"]		= color::color3c_t( 64, 64, 64);
        colors_["node_bg"]			= color::color3c_t( 150, 150, 150);
        colors_["node_picked_bg"]	= color::color3c_t( 200, 140, 0);
        colors_["node_error_bg"]	= color::color3c_t( 150, 25, 0);

        colors_["node_active_out"]  = color::color3c_t( 255, 0, 0);
        colors_["node_context_out"] = color::color3c_t( 0, 0, 255);

        // manipulators
        colors_["manipulator"]			= color::color3c_t( 255, 255, 0);
        colors_["active_manipulator"]	= color::color3c_t( 255, 0, 0);

        colors_["x_axis"] = color::color3c_t( 255, 0, 0);
        colors_["y_axis"] = color::color3c_t( 0, 255, 0);
        colors_["z_axis"] = color::color3c_t( 0, 0, 255);

        // interface elems
        colors_["grid"] = color::color3c_t( 132, 132, 132);
        colors_["box_pick"] = color::color3c_t( 255, 0, 0);
    }

    const color::color3c_t& get_color( const std::string& k) const
    {
        std::map<std::string, color::color3c_t>::const_iterator it;
        it = colors_.find( k);

        if( it != colors_.end())
            return it->second;

        #ifndef NDEBUG
            std::cout << "Palette: color not found " << k << "\n";
        #endif

        return error_color_;
    }

    std::map<std::string, color::color3c_t> colors_;
    color::color3c_t error_color_;
};

palette_t& palette_t::instance()
{
    static palette_t pal;
    return pal;
}

palette_t::palette_t() { pimpl_.reset( new impl());}

palette_t::~palette_t()
{
    // do not remove. It's needed by auto_ptr.
}

void palette_t::rebuild() { pimpl_->rebuild_palette();}

const color::color3c_t& palette_t::color( const std::string& k) const
{
    return pimpl_->get_color( k);
}

QColor palette_t::qcolor( const std::string& k) const
{
    color::color3c_t c = pimpl_->get_color( k);
    return QColor( c.x, c.y, c.z, 255);
}

bool palette_t::find_color( const std::string& k) const
{
    // TODO: implement this.
    RAMEN_ASSERT( 0);
}

void palette_t::set_color( const std::string& k, const color::color3c_t& c)
{
    // TODO: implement this.
    RAMEN_ASSERT( 0);
}

void palette_t::set_color( const std::string& k, const QColor& c)
{
    // TODO: implement this.
    RAMEN_ASSERT( 0);
}

} // ui
} // ramen
