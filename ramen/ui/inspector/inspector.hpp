// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_INSPECTOR_HPP
#define	RAMEN_UI_INSPECTOR_HPP

#include<ramen/ui/inspector/inspector_fwd.hpp>

#include<QWidget>

#include<ramen/nodes/node_fwd.hpp>

#include<ramen/ui/inspector/panel_factory.hpp>

class QScrollArea;
class QPushButton;
class QStackedWidget;
class QLineEdit;

namespace ramen
{
namespace ui
{

class inspector_t : public QWidget
{
    Q_OBJECT

public:

    inspector_t();
    ~inspector_t();

    // metrics
    int left_margin() const;
    int width() const;

    void edit_node( nodes::node_t *n);

    void update_state();

    panel_factory_t& panel_factory() { return factory_;}

private Q_SLOTS:

    void rename_node();
    void show_help();

private:

    inspector_t( const inspector_t& other);
    void operator=( const inspector_t& other);

    void create_header();

    void recreate_panel( nodes::node_t *n);

    void update_header_widgets();

    void clear_panel();
    void set_panel( QWidget *panel);

    // metrics
    mutable int left_margin_;
    mutable int width_;

    QScrollArea *scroll_;
    QWidget *header_;
    QLineEdit *name_edit_;
    QPushButton *help_;
    QStackedWidget *view_;

    panel_factory_t factory_;
    panel_factory_t::iterator current_;
};

} // ui
} // ramen

#endif
