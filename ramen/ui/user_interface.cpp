// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/user_interface.hpp>

#include<string>
#include<utility>
#include<iostream>

#include<boost/foreach.hpp>
#include<boost/thread.hpp>
#include<boost/filesystem/operations.hpp>
#include<boost/tokenizer.hpp>

#include<OpenImageIO/imageio.h>

#include<QApplication>
#include<QSplashScreen>
#include<QMessageBox>
#include<QFile>
#include<QFileDialog>
#include<QGridLayout>
#include<QHBoxLayout>
#include<QCheckBox>
#include<QKeyEvent>
#include<QPlastiqueStyle>

#include<ramen/assert.hpp>

#include<ramen/core/exceptions.hpp>

#include<ramen/system/system.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>
#include<ramen/app/preferences.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/memory/manager.hpp>

#include<ramen/qwidgets/time_slider.hpp>

#include<ramen/ui/main_window.hpp>
#include<ramen/ui/palette.hpp>

#include<ramen/ui/anim/anim_editor.hpp>

#include<ramen/ui/inspector/inspector.hpp>

namespace ramen
{
namespace ui
{

user_interface_t::user_interface_t() : QObject()
{
    rendering_ = false;
    cancelled_ = false;
    interacting_ = false;
    event_filter_installed_ = false;
    window_ = 0;
    init_image_types_string();
}

user_interface_t::~user_interface_t()
{
}

void user_interface_t::init()
{
    init_ui_style();
    create_new_document();
    window_ = new main_window_t();
    restore_window_state();
}

void user_interface_t::init_ui_style()
{
    QPlastiqueStyle *style = new QPlastiqueStyle();
    qApp->setStyle( style);

    // init palette
    palette_t::instance();
}

void user_interface_t::save_window_state()
{
    QByteArray window_state = window_->saveState();
    boost::filesystem::path p = app().system().application_user_path() / "wstate.ui";
    QFile file( p.string().c_str());

    if( file.open( QIODevice::WriteOnly))
        file.write( window_state);
}

void user_interface_t::restore_window_state()
{
    boost::filesystem::path p = app().system().application_user_path() / "wstate.ui";
    {
        QFile file( p.string().c_str());

        if( file.open( QIODevice::ReadOnly))
        {
            QByteArray window_state = file.readAll();
            window_->restoreState( window_state);
            return;
        }
    }
}

void user_interface_t::show() { window_->show();}

int user_interface_t::run( const boost::filesystem::path& p)
{
    if( !p.empty())
        open_document( p);

    return qApp->exec();
}

void user_interface_t::quit()
{
    app().set_quitting( true);
    save_window_state();
    qApp->quit();
}

const inspector_t& user_interface_t::inspector() const
{
    RAMEN_ASSERT( main_window());

    return main_window()->inspector();
}

inspector_t& user_interface_t::inspector()
{
    RAMEN_ASSERT( main_window());

    return main_window()->inspector();
}

const anim_editor_t& user_interface_t::anim_editor() const
{
    RAMEN_ASSERT( main_window());

    return main_window()->anim_editor();
}

anim_editor_t& user_interface_t::anim_editor()
{
    RAMEN_ASSERT( main_window());

    return main_window()->anim_editor();
}

void user_interface_t::create_new_document()
{
    //set_active_node( 0);
    //set_context_node( 0);

    if( main_window())
        anim_editor().clear_all();

    app().create_new_document();

    //render_composition_dialog_t::instance().set_frame_range( app().document().world_node().start_frame(),
    //                                                         app().document().world_node().end_frame());

    //render_composition_dialog_t::instance().set_mblur_settings( 0, 1);
    update();
}

void user_interface_t::open_document( const boost::filesystem::path& p)
{
    create_new_document();
    /*
    boost::filesystem::ifstream ifs( p, serialization::yaml_iarchive_t::file_open_mode());

    if( !ifs.is_open() || !ifs.good())
    {
        error( std::string( "Couldn't open input file ") + filesystem::file_string( p));
        update();
        return;
    }

    std::auto_ptr<serialization::yaml_iarchive_t> in;

    try
    {
        in.reset( new serialization::yaml_iarchive_t( ifs));

        if( !in->read_composition_header())
        {
            create_new_document();
            error( std::string( "Couldn't open input file ") + filesystem::file_string( p));
            update();
            return;
        }

        app().document().set_file( p);
        app().document().load( *in);
        main_window()->update_recent_files_menu( p);
    }
    catch( std::exception& e)
    {
        create_new_document();
        error( std::string( "Couldn't open input file ") + filesystem::file_string( p));
        update();
        return;
    }

    // read here ui info

    // update the dialogs
    render_composition_dialog_t::instance().set_frame_range( app().document().world_node().start_frame(),
                                                             app().document().world_node().end_frame());

    render_composition_dialog_t::instance().set_mblur_settings( 0, 1);

    update();
    std::string err = in->errors();

    if( !err.empty())
        multiline_alert_t::instance().show_alert( "Errors during file open", err);
    */
}

bool user_interface_t::save_document()
{
    RAMEN_ASSERT( app().document().has_file());

    try
    {
        /*
        serialization::yaml_oarchive_t out;
        out.write_composition_header();
        app().document().save( out);
        write_ui_state( out);

        out.write_to_file( app().document().file());
        app().document().set_dirty( false);
        */
    }
    catch( core::exception& e)
    {
        error( core::make_string( "Couldn't save file. Exception, what = ", e.what()));
        return false;
    }
    catch( std::exception& e)
    {
        error( core::make_string( "Couldn't save file. Exception, what = ", e.what()));
        return false;
    }

    return true;
}

void user_interface_t::node_released( nodes::node_t *n)
{
}

void user_interface_t::update()
{
    if( !app().quitting())
    {
        if( window_)
            window_->update_state();

        update_anim_editors();
    }
}

int user_interface_t::start_frame() const
{
    return app().document().world_node().start_frame();
}

int user_interface_t::end_frame() const
{
    return app().document().world_node().end_frame();
}

float user_interface_t::frame() const
{
    return app().document().world_node().frame();
}

void user_interface_t::set_start_frame( int t)
{
    app().document().world_node().set_start_frame( t);
    main_window()->time_slider().update_state( app().document().world_node().start_frame(),
                                               app().document().world_node().frame(),
                                               app().document().world_node().end_frame());

    //render_composition_dialog_t::instance().set_frame_range( app().document().world_node().start_frame(),
    //                                                         app().document().world_node().end_frame());
}

void user_interface_t::set_end_frame( int t)
{
    app().document().world_node().set_end_frame( t);
    main_window()->time_slider().update_state( app().document().world_node().start_frame(),
                                               app().document().world_node().frame(),
                                               app().document().world_node().end_frame());

    //render_composition_dialog_t::instance().set_frame_range( app().document().world_node().start_frame(),
    //                                                         app().document().world_node().end_frame());
}

void user_interface_t::set_frame( double t)
{
    app().document().world_node().set_frame( t);
    main_window()->time_slider().update_state( app().document().world_node().start_frame(),
                                               app().document().world_node().frame(),
                                               app().document().world_node().end_frame());

    inspector().update_state();
    update_anim_editors();
    //viewer().frame_changed();
}

void user_interface_t::update_anim_editors()
{
    if( main_window())
        anim_editor().update_state();
}

void user_interface_t::fatal_error( const core::string8_t& msg) const
{
    QMessageBox::critical( 0, "Fatal Error", msg.c_str());
}

void user_interface_t::error( const core::string8_t& msg) const
{
    QMessageBox::warning( ( QWidget *) main_window(), "Error", msg.c_str());
}

void user_interface_t::inform( const core::string8_t& msg) const
{
    QMessageBox::information( ( QWidget *) main_window(), "Info", msg.c_str());
}

bool user_interface_t::question( const core::string8_t& what, bool default_answer) const
{
    QMessageBox::StandardButton result;

    if( default_answer)
    {
        result = QMessageBox::question( ( QWidget *)  main_window(), "Question", what.c_str(),
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        return result == QMessageBox::Yes;
    }
    else
    {
        result = QMessageBox::question( ( QWidget *) main_window(), "Question", what.c_str(),
                                        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
        return result == QMessageBox::Yes;
    }
}

bool user_interface_t::image_sequence_file_selector( boost::filesystem::path& p,
                                                     bool& sequence,
                                                     bool& relative) const
{
    core::string8_t types( image_types_string().toStdString().c_str());
    return image_sequence_file_selector( "Open Image", types, p, sequence, relative);
}

bool user_interface_t::image_sequence_file_selector( const core::string8_t& title,
                                                     const core::string8_t& types,
                                                     boost::filesystem::path& p,
                                                     bool& sequence,
                                                     bool& relative) const
{
    static bool was_relative = false;
    static bool was_sequence = true;

    QFileDialog dialog( 0, title.c_str(), QString::null, types.c_str());
    dialog.setOption( QFileDialog::DontUseNativeDialog, true);
    dialog.setFileMode( QFileDialog::ExistingFile);

    QWidget *extra_widgets = new QWidget( &dialog);
    QHBoxLayout *layout = new QHBoxLayout( extra_widgets);

    QCheckBox *relative_check = new QCheckBox( "Relative");

    if( app().document().has_file())
    {
        RAMEN_ASSERT( !app().document().world_node().composition_dir().empty());
        relative_check->setChecked( was_relative);
    }
    else
    {
        was_relative = false;
        relative_check->setChecked( false);
        relative_check->setEnabled( false);
    }

    QCheckBox *sequence_check = new QCheckBox( "Sequence");
    sequence_check->setChecked( was_sequence);

    layout->addWidget( relative_check);
    layout->addWidget( sequence_check);

    QGridLayout *glayout = (QGridLayout *) dialog.layout();
    glayout->addWidget( extra_widgets, glayout->rowCount(), 0, 1, glayout->columnCount(), Qt::AlignLeft);

    dialog.show();

    QStringList filenames;

    if( dialog.exec())
    {
        filenames = dialog.selectedFiles();
        p = filenames[0].toStdString();

        relative = relative_check->isChecked();
        was_relative = relative;

        if( relative_check->isChecked())
        {
            boost::filesystem::path dir = app().document().file().parent_path();
            QDir qdir( QString( p.string().c_str()));
            QString fname( QString( dir.string().c_str()));
            QString rel_path( qdir.relativeFilePath( fname));
            p =boost::filesystem::path( rel_path.toStdString());
        }

        sequence = sequence_check->isChecked();
        was_sequence = sequence;
        return true;
    }

    return false;
}

// event filter
void user_interface_t::start_long_process()
{
    RAMEN_ASSERT( !event_filter_installed_);

    cancelled_ = false;
    qApp->installEventFilter( this);
    event_filter_installed_ = true;
}

void user_interface_t::process_events() { qApp->processEvents();}

bool user_interface_t::process_cancelled() const	{ return cancelled_;}

void user_interface_t::end_long_process()
{
    RAMEN_ASSERT( event_filter_installed_);

    qApp->removeEventFilter( this);
    event_filter_installed_ = false;
}

bool user_interface_t::eventFilter( QObject *watched, QEvent *event)
{
    switch( event->type())
    {
    case QEvent::KeyPress:
    {
        QKeyEvent *key_event = dynamic_cast<QKeyEvent*>( event);

        if( key_event->key() == Qt::Key_Escape)
            cancelled_ = true;

        return true;
    }
    break;

    case QEvent::KeyRelease:
    case QEvent::MouseButtonPress:
    case QEvent::MouseButtonRelease:
        return true;

    case QEvent::MouseMove:
    {
        QMouseEvent *mouse_event = dynamic_cast<QMouseEvent*>( event);
        return mouse_event->buttons() == Qt::NoButton;
    }
    break;

    default:
        return false; // pass all other events
    }
}

QFont user_interface_t::get_fixed_width_code_font()
{
    QFont font;
    font.setFamily( "Courier");
    font.setFixedPitch( true);
    font.setPointSize( 11);
    return font;
}

void user_interface_t::init_image_types_string()
{
    image_types_str_ = "Image Files (";

    // TODO: this is crashing. investigate why.
    std::string list;
    if( OIIO::getattribute( "extension_list", list))
    {
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

        boost::char_separator<char> colon_sep( ";");
        tokenizer fmt_tokens( list, colon_sep);
        for( tokenizer::iterator fmt_iter = fmt_tokens.begin(); fmt_iter != fmt_tokens.end(); ++fmt_iter)
        {
            std::string format_and_exts = *fmt_iter;
            std::size_t s = format_and_exts.find( ':');
            std::string exts( format_and_exts, s + 1, std::string::npos);

            boost::char_separator<char> comma_sep( ",");
            tokenizer ext_tokens( exts, comma_sep);
            for( tokenizer::iterator ext_iter = ext_tokens.begin(); ext_iter != ext_tokens.end(); ++ext_iter)
            {
                image_types_str_.append( "*.");
                image_types_str_.append( QString::fromStdString( *ext_iter));
                image_types_str_.append( " ");
            }
        }
    }

    image_types_str_.append( ")");
}

} // ui
} // ramen
