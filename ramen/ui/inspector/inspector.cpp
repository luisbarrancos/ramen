// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/inspector/inspector.hpp>

#include<boost/bind.hpp>

#include<QApplication>
#include<QFontMetrics>
#include<QVBoxLayout>
#include<QLabel>
#include<QScrollArea>
#include<QFrame>
#include<QPushButton>
#include<QMessageBox>
#include<QStackedWidget>
#include<QDoubleSpinBox>
#include<QLineEdit>

#include<ramen/nodes/node.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/string_algo/valid_c_identifier.hpp>

#include<ramen/ui/user_interface.hpp>

#include<ramen/ui/anim/anim_editor.hpp>

#include<ramen/ui/inspector/panel.hpp>

namespace ramen
{
namespace ui
{
namespace
{

class rename_node_command_t : public undo::command_t
{
public:

    rename_node_command_t( node_t *n,
                           const core::string8_t& new_name,
                           QLineEdit *name_edit) : undo::command_t( "Rename Node")
    {
        n_ = n;
        old_name_ = n_->name();
        new_name_ = new_name;
        name_edit_ = name_edit;
    }

    virtual void undo()
    {
        rename( old_name_);
        undo::command_t::undo();
    }

    virtual void redo()
    {
        rename( new_name_);
        undo::command_t::redo();
    }

private:

    void rename( const core::string8_t& name)
    {
        name_edit_->blockSignals( true);
        n_->set_name( name.c_str());

        if( n_->is_active())
        {
            name_edit_->setText( n_->name().c_str());
            app().ui()->anim_editor().node_renamed( n_);
        }

        app().ui()->update();
        name_edit_->setModified( false);
        name_edit_->blockSignals( false);
    }

    node_t *n_;
    core::string8_t new_name_;
    core::string8_t old_name_;

    QLineEdit *name_edit_;
};

} // unnamed

inspector_t::inspector_t() : QWidget(), left_margin_( 0), width_( 0)
{
    QVBoxLayout *layout = new QVBoxLayout();
    layout->setContentsMargins( 0, 0, 0, 0);

    setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Preferred);
    setMinimumSize( width()+16, 0);
    setMaximumSize( width()+16, QWIDGETSIZE_MAX);
    setWindowTitle( "Inspector");
    setContentsMargins( 0, 0, 0, 0);
    scroll_ = new QScrollArea();
    scroll_->setVerticalScrollBarPolicy( Qt::ScrollBarAsNeeded);
    scroll_->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff);
    scroll_->setWidgetResizable( true);
    scroll_->setContentsMargins( 0, 0, 0, 0);

    QVBoxLayout *layout2 = new QVBoxLayout();
    layout2->setContentsMargins( 0, 0, 0, 0);

    QWidget *top = new QWidget();
    create_header();
    layout2->addWidget( header_);

    QFrame *separator = new QFrame();
    separator->setFrameStyle( QFrame::HLine | QFrame::Raised);
    separator->setLineWidth( 1);
    layout2->addWidget( separator);

    view_ = new QStackedWidget();
    layout2->addWidget( view_);
    top->setLayout( layout2);
    scroll_->setWidget( top);

    layout->addWidget( scroll_);
    setLayout( layout);
    current_ = factory_.end();
}

inspector_t::~inspector_t()
{
}

int inspector_t::left_margin() const
{
    if( !left_margin_)
    {
        QLabel *tmp = new QLabel();
        tmp->setText( "M");
        QSize s = tmp->sizeHint();
        delete tmp;

        const int max_label_lenght = 16;
        left_margin_ = s.width() * max_label_lenght;
    }

    return left_margin_;
}

int inspector_t::width() const
{
    if( !width_)
    {
        QDoubleSpinBox *tmp = new QDoubleSpinBox();
        QSize s = tmp->sizeHint();
        delete tmp;

        width_ = left_margin() + 5 + ( 3 * s.height()) + ( 3 * s.width()) + 30;
    }

    return width_;
}

void inspector_t::create_header()
{
    header_ = new QWidget();

    name_edit_ = new QLineEdit( header_);

    help_ = new QPushButton( header_);
    help_->setText( "Help");
    QSize s = help_->sizeHint();

    int hpos = left_margin() / 2;
    int vsize = name_edit_->sizeHint().height();

    QLabel *label = new QLabel( header_);
    label->setText( "Name");
    label->move( 0, 5);
    label->resize( hpos - 5, vsize);
    label->setAlignment( Qt::AlignRight | Qt::AlignVCenter);

    name_edit_->move( hpos, 5);
    name_edit_->resize( width() - hpos - 15 - s.width() - 5, vsize);
    name_edit_->setEnabled( false);
    connect( name_edit_, SIGNAL( editingFinished()), this, SLOT( rename_node()));
    hpos = hpos + width() - hpos - 15 - s.width();

    help_->move( hpos, 5);
    help_->setEnabled( false);
    connect( help_, SIGNAL( clicked()), this, SLOT( show_help()));

    header_->setMinimumSize( width(), vsize + 10);
    header_->setMaximumSize( width(), vsize + 10);
    header_->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void inspector_t::edit_node( node_t *n)
{
    if( app().quitting())
        return;

    if( current_ != factory_.end())
    {
        clear_panel();
        current_ = factory_.end();
    }

    if( n != 0)
    {
        current_ = factory_.create_panel( n);
        set_panel( current_->second->widget());
    }

    update_header_widgets();
}

void inspector_t::clear_panel()
{
    if( view_->count())
        view_->removeWidget( view_->currentWidget());
}

void inspector_t::set_panel( QWidget *panel)
{
    clear_panel();

    if( panel)
    {
        view_->addWidget( panel);
        view_->setCurrentIndex( 0);
    }
}

void inspector_t::update()
{
    if( current_ != factory_.end())
        current_->second->update();
}

void inspector_t::recreate_panel( node_t *n)
{
    edit_node( 0);
    factory_.delete_panel( n);
    edit_node( n);
}

void inspector_t::update_header_widgets()
{
    name_edit_->blockSignals( true);

    node_t *n = app().ui()->active_node();

    if( n)
    {
        name_edit_->setText( n->name().c_str());
        name_edit_->setEnabled( true);
        help_->setEnabled( n->help_string());
    }
    else
    {
        name_edit_->setText( "");
        name_edit_->setEnabled( false);
        help_->setEnabled( false);
    }

    name_edit_->blockSignals( false);
}

void inspector_t::rename_node()
{
    if( name_edit_->isModified())
    {
        node_t *n = app().ui()->active_node();
        RAMEN_ASSERT( n);

        core::string8_t new_name = name_edit_->text().toStdString().c_str();

        if( string_algo::is_valid_c_identifier( new_name))
        {
            core::auto_ptr_t<rename_node_command_t> c( new rename_node_command_t( n, new_name, name_edit_));
            c->redo();
            app().document().undo_stack().push_back( boost::move( c));
            app().ui()->update();
        }
        else
        {
            name_edit_->blockSignals( true);
            name_edit_->setText( n->name().c_str());
            name_edit_->blockSignals( false);
        }
    }
}

void inspector_t::show_help()
{
    node_t *n = app().ui()->active_node();

    RAMEN_ASSERT( n);
    RAMEN_ASSERT( n->help_string());

    QMessageBox msg_box;
    msg_box.setWindowTitle( "Help");
    msg_box.setText( n->help_string());
    msg_box.exec();
}

} // ui
} // ramen
