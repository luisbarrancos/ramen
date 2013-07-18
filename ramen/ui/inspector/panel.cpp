// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/inspector/panel.hpp>

#include<boost/foreach.hpp>

#include<QVBoxLayout>

#include<ramen/ui/inspector/inspector.hpp>

#include<ramen/nodes/node.hpp>
#include<ramen/ui/user_interface.hpp>

namespace ramen
{
namespace ui
{

panel_t::panel_t( node_t *n) : n_( n), panel_( 0)
{
    panel_ = new QWidget();

    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins( 0, 0, 0, 0);
    layout->setSizeConstraint( QLayout::SetFixedSize);

    BOOST_FOREACH( param_t& param, n_->param_set())
    {
        QWidget *w = param.create_widgets();

        if( w)
            layout->addWidget( w);
    }

    layout->addStretch();
    panel_->setLayout( layout);
}

panel_t::~panel_t()
{
    connection_.disconnect();
    panel_->deleteLater();
}

void panel_t::set_connection( const boost::signals2::connection c)
{
    connection_ = c;
}

void panel_t::update() { n_->update_widgets();}

} // ui
} // ramen
