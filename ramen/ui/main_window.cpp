// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/ui/main_window.hpp>

#include<boost/foreach.hpp>

#include<QApplication>
#include<QMenuBar>
#include<QMenu>
#include<QAction>
#include<QMessageBox>
#include<QVBoxLayout>
#include<QCloseEvent>
#include<QFileDialog>
#include<QDesktopServices>
#include<QDockWidget>
#include<QDesktopWidget>
#include<QUrl>
#include<QStatusBar>
#include<QSettings>
#include<QStringList>
#include<QFileInfo>
#include<QToolBar>
#include<QFrame>

#include<ramen/version.hpp>

#include<ramen/app/application.hpp>
#include<ramen/app/document.hpp>

#include<ramen/memory/manager.hpp>

#include<ramen/nodes/node.hpp>
#include<ramen/nodes/node_factory.hpp>
#include<ramen/nodes/node_graph_modifier.hpp>
#include<ramen/nodes/viewer_node.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/qwidgets/time_slider.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/time_controls.hpp>

#include<ramen/ui/anim/anim_editor.hpp>
#include<ramen/ui/inspector/inspector.hpp>
#include<ramen/ui/nodes_view/nodes_view_tabs.hpp>
#include<ramen/ui/nodes_view/nodes_view.hpp>
#include<ramen/ui/viewer/viewer_tabs.hpp>

namespace ramen
{
namespace ui
{

const int main_window_t::max_recently_opened_files = 5;

const char *main_window_t::document_extension()
{
    return ".rmn";
}

const char *main_window_t::file_dialog_extension()
{
    return "Ramen Composition (*.rmn)";
}

main_window_t::main_window_t() : QMainWindow()
{
    time_slider_ = 0;
    nodes_view_tabs_ = 0;
    inspector_ = 0;
    time_controls_ = 0;
    viewer_tabs_ = 0;

    menubar_ = menuBar();

    recently_opened_.assign( max_recently_opened_files, (QAction *) 0);

    time_controls_ = new time_controls_t();

    create_actions();
    create_menus();

    // docks
    setCorner( Qt::TopRightCorner, Qt::RightDockWidgetArea);
    setCorner( Qt::BottomRightCorner, Qt::RightDockWidgetArea);

    // Inspector
    inspector_ = new inspector_t();
    inspector_dock_ = new QDockWidget( "Inspector", this);
    inspector_dock_->setObjectName( "inspector_dock");
    inspector_dock_->setAllowedAreas( Qt::RightDockWidgetArea |
                                      Qt::LeftDockWidgetArea  |
                                      Qt::BottomDockWidgetArea);
    inspector_dock_->setWidget( inspector_);
    add_dock_widget( Qt::RightDockWidgetArea, inspector_dock_);

    // anim editor dock
    anim_editor_ = new anim_editor_t();
    anim_editor_dock_ = new QDockWidget( "Curve Editor", this);
    anim_editor_dock_->setObjectName( "anim_editor_dock");
    anim_editor_dock_->setAllowedAreas( Qt::BottomDockWidgetArea |
                                        Qt::TopDockWidgetArea    |
                                        Qt::LeftDockWidgetArea   |
                                        Qt::RightDockWidgetArea);
    anim_editor_dock_->setWidget( anim_editor_);
    add_dock_widget( Qt::BottomDockWidgetArea, anim_editor_dock_);

    // node graph view
    {
        node_graph_dock_ = new QDockWidget( "Node Graph", this);
        node_graph_dock_->setObjectName( "node_graph_dock");
        node_graph_dock_->setAllowedAreas( Qt::BottomDockWidgetArea |
                                           Qt::TopDockWidgetArea    |
                                           Qt::LeftDockWidgetArea   |
                                           Qt::RightDockWidgetArea);


        QWidget *nodes_view_tabs_ = new nodes_view_tabs_t();
        node_graph_dock_->setWidget( nodes_view_tabs_);
        add_dock_widget( Qt::LeftDockWidgetArea, node_graph_dock_);
    }

    // viewer
    viewer_tabs_ = new viewer_tabs_t();
    setCentralWidget( viewer_tabs_);

    // while testing
    viewer_tabs_->add_viewer( "Viewer1");

    // time toolbar
    addToolBar( Qt::BottomToolBarArea, create_time_toolbar());

    // create the status bar
    statusBar()->showMessage( RAMEN_NAME_FULL_VERSION_STR);

    QRect screen_size = qApp->desktop()->availableGeometry();
    move( screen_size.left(), screen_size.top());
    resize( screen_size.width(), screen_size.height() - 40);
    setWindowTitle( "Ramen");

    setWindowIcon( QIcon( ":small_app_icon.png"));
}

QToolBar *main_window_t::create_time_toolbar()
{
    QToolBar *toolbar = new QToolBar( "Time Controls");
    toolbar->setObjectName( "time_controls");
    toolbar->setFloatable( false);
    toolbar->setMovable( false);

    time_slider_ = new qwidgets::time_slider_t();
    time_slider_->setSizePolicy( QSizePolicy::Expanding, time_slider_->sizePolicy().verticalPolicy());
    connect( time_slider_, SIGNAL( start_frame_changed( int)), app().ui(), SLOT( set_start_frame( int)));
    connect( time_slider_, SIGNAL( end_frame_changed( int)), app().ui(), SLOT( set_end_frame( int)));
    connect( time_slider_, SIGNAL( frame_changed( double)), app().ui(), SLOT( set_frame( double)));

    toolbar->addWidget( time_slider_);
    toolbar->addSeparator();

    time_controls_->setSizePolicy( QSizePolicy::Fixed, time_controls_->sizePolicy().verticalPolicy());
    toolbar->addWidget( time_controls_);
    return toolbar;
}

void main_window_t::add_dock_widget( Qt::DockWidgetArea area, QDockWidget *dock)
{
    addDockWidget( area, dock);
    view_->addAction( dock->toggleViewAction());
}

void main_window_t::closeEvent( QCloseEvent *event)
{
    quit();

    if( app().quitting())
        event->accept();
    else
        event->ignore();
}

void main_window_t::create_actions()
{
    new_ = new QAction( "New", this);
    new_->setShortcut( QString( "Ctrl+N"));
    new_->setShortcutContext( Qt::ApplicationShortcut);
    connect( new_, SIGNAL( triggered()), this, SLOT( new_document()));

    open_ = new QAction( "Open...", this);
    open_->setShortcut( QString( "Ctrl+O"));
    open_->setShortcutContext( Qt::ApplicationShortcut);
    connect( open_, SIGNAL( triggered()), this, SLOT( open_document()));

    for( int i = 0; i < max_recently_opened_files; ++i)
    {
        recently_opened_[i] = new QAction( this);
        recently_opened_[i]->setVisible( false);
        connect( recently_opened_[i], SIGNAL( triggered()), this, SLOT( open_recent_document()));
    }

    save_ = new QAction( "Save", this);
    save_->setShortcut( QString( "Ctrl+S"));
    save_->setShortcutContext( Qt::ApplicationShortcut);
    connect( save_, SIGNAL( triggered()), this, SLOT( save_document()));

    save_as_ = new QAction( "Save As...", this);
    connect( save_as_, SIGNAL( triggered()), this, SLOT( save_document_as()));

    quit_ = new QAction( "Quit", this);
    quit_->setShortcut( QString( "Ctrl+Q"));
    quit_->setShortcutContext( Qt::ApplicationShortcut);
    connect( quit_, SIGNAL( triggered()), this, SLOT( quit()));

    undo_ = new QAction( "Undo", this);
    undo_->setShortcut( QString( "Ctrl+Z"));
    undo_->setShortcutContext( Qt::ApplicationShortcut);
    connect( undo_, SIGNAL( triggered()), this, SLOT( undo()));

    redo_ = new QAction( "Redo", this);
    redo_->setShortcut( QString( "Ctrl+Shift+Z"));
    redo_->setShortcutContext( Qt::ApplicationShortcut);
    connect( redo_, SIGNAL( triggered()), this, SLOT( redo()));

    preferences_ = new QAction( "Preferences...", this);
    connect( preferences_, SIGNAL( triggered()), this, SLOT( show_preferences_dialog()));

    create_viewer_ = new QAction( "Create Viewer", this);
    connect( create_viewer_, SIGNAL( triggered()), this, SLOT( create_viewer()));

    about_ = new QAction( "About Ramen...", this);
    connect( about_, SIGNAL( triggered()), this, SLOT( show_about_box()));

    project_web_ = new QAction( tr( "Project Website..."), this);
    connect( project_web_, SIGNAL( triggered()), this, SLOT( go_to_project_website()));

    // non-menu actions
    next_frame_ = new QAction( this);
    next_frame_->setShortcut( Qt::Key_Right);
    next_frame_->setShortcutContext( Qt::WidgetWithChildrenShortcut);
    connect( next_frame_, SIGNAL( triggered()), &(time_controls()), SLOT( next_frame()));
    addAction( next_frame_);

    prev_frame_ = new QAction( this);
    prev_frame_->setShortcut( Qt::Key_Left);
    prev_frame_->setShortcutContext( Qt::WidgetWithChildrenShortcut);
    connect( prev_frame_, SIGNAL( triggered()), &(time_controls()), SLOT( prev_frame()));
    addAction( prev_frame_);
}

void main_window_t::create_menus()
{
    file_ = menubar_->addMenu( "File");
    file_->addAction( new_);
    file_->addAction( open_);

    open_recent_ = file_->addMenu( "Open Recent");
    for( int i = 0; i < max_recently_opened_files; ++i)
        open_recent_->addAction( recently_opened_[i]);

    init_recent_files_menu();

    file_->addAction( save_);
    file_->addAction( save_as_);
    file_->addSeparator();

    import_ = file_->addMenu( "Import");
    export_ = file_->addMenu( "Export");

    file_->addSeparator();
    file_->addAction( quit_);

    edit_ = menubar_->addMenu( "Edit");
    edit_->addAction( undo_);
    edit_->addAction( redo_);
    edit_->addSeparator();
    edit_->addAction( preferences_);

    view_ = menubar_->addMenu( "View");
    view_->addAction( create_viewer_);
    view_->addSeparator();

    help_ = menubar_->addMenu( "Help");
    help_->addAction( about_);
    help_->addAction( project_web_);
}

bool main_window_t::can_close_document()
{
    if( app().document().dirty())
    {
        int r = QMessageBox::warning( this,
                                      "Ramen",
                                      "The document has been modified.\n"
                                      "Do you want to save your changes?",
                                      QMessageBox::Yes | QMessageBox::Default,
                                      QMessageBox::No,
                                      QMessageBox::Cancel | QMessageBox::Escape);

        if (r == QMessageBox::Yes)
        {
            save_document();
            return app().document().dirty();
        }
        else
            if( r == QMessageBox::Cancel)
                return false;
    }

    return true;
}

void main_window_t::new_document()
{
    if( can_close_document())
        app().ui()->create_new_document();
}

void main_window_t::open_document()
{
    if( can_close_document())
    {
        QString fname = QFileDialog::getOpenFileName( 0,
                                                      "Open Composition",
                                                      QString::null,
                                                      file_dialog_extension(),
                                                      0,
                                                      QFileDialog::DontUseNativeDialog);

        if( !( fname.isEmpty()))
        {
            boost::filesystem::path p( fname.toStdString());
            app().ui()->open_document( p);
        }
    }
}

void main_window_t::open_recent_document()
{
    if( can_close_document())
    {
        QAction *action = qobject_cast<QAction *>(sender());

        if( action)
        {
            QString fname = action->data().toString();
            boost::filesystem::path p( fname.toStdString());
            app().ui()->open_document( p);
        }
    }
}

void main_window_t::save_document()
{
    if( app().document().has_file())
    {
        app().ui()->save_document();
        app().ui()->update();
    }
    else
        save_document_as();
}

void main_window_t::save_document_as()
{
    QString fname = QFileDialog::getSaveFileName( 0,
                                                  "Save Composition As",
                                                  QString::null,
                                                  file_dialog_extension(),
                                                  0,
                                                  QFileDialog::DontUseNativeDialog);

    if( !fname.isEmpty())
    {
        boost::filesystem::path p( fname.toStdString());

        if( p.extension() == std::string())
            p.replace_extension( document_extension());

        boost::filesystem::path old_file = app().document().file();
        app().document().set_file( p);

        if( !app().ui()->save_document())
        {
            // save was not successful, restore the relative paths
            // to the state before trying to save.
            if( !old_file.empty())
                app().document().set_file( old_file);
        }
        else
        {
            update_recent_files_menu( app().document().file());
            app().document().undo_stack().clear_all();
        }

        app().ui()->update();
    }
}

void main_window_t::quit()
{
    if( app().document().dirty())
    {
        int r = QMessageBox::warning( this,
                                      "Ramen",
                                      "The document has been modified.\n"
                                      "Do you want to save your changes before quitting?",
                                      QMessageBox::Yes | QMessageBox::Default,
                                      QMessageBox::No,
                                      QMessageBox::Cancel | QMessageBox::Escape);

        switch( r)
        {
            case QMessageBox::Yes:
            {
                save_document();

                // if the document is still dirty, it means
                // save was cancelled, so we return without quitting
                if( app().document().dirty())
                    return;
            }
            break;

            case QMessageBox::No:
                app().ui()->quit();
            break;

            case QMessageBox::Cancel:
            return;
        }
    }

    app().ui()->quit();
}

void main_window_t::undo()
{
    app().document().undo_stack().undo();
    app().ui()->update();
}

void main_window_t::redo()
{
    app().document().undo_stack().redo();
    app().ui()->update();
}

void main_window_t::show_preferences_dialog()
{
    //preferences_dialog_t::instance().exec_dialog();
}

void main_window_t::create_viewer()
{
    nodes::composite_node_t *n = nodes_view_tabs().active_view()->node();
    nodes::node_graph_modifier_t m( n, "Create viewer");
    m.create_node( nodes::g_viewer_node_id, true);
    m.execute( true);
}

void main_window_t::show_about_box() {}

void main_window_t::go_to_project_website() {}

void main_window_t::update_state()
{
    if( app().document().dirty())
        setWindowTitle( "Ramen *");
    else
        setWindowTitle( "Ramen");

    update_menus();
    time_slider_->update_state( app().document().world_node().start_frame(),
                                app().document().world_node().frame(),
                                app().document().world_node().end_frame());

    nodes_view_tabs_->update_state();
    viewer_tabs_->update_state();
    time_controls_->update_state();
}

void main_window_t::update_recent_files_menu( const boost::filesystem::path& p)
{
    QString fileName( p.string().c_str());

    QSettings settings( "com.ramen.qt", "Ramen Recent Files");
    QStringList files = settings.value( "recent_file_list").toStringList();
    files.removeAll( fileName);
    files.prepend( fileName);

    while( files.size() > max_recently_opened_files)
        files.removeLast();

    settings.setValue("recent_file_list", files);

    int num_recent_files = std::min( files.size(), (int) max_recently_opened_files);

    for( int i = 0; i < num_recent_files; ++i)
    {
        QString stripped = QFileInfo( files[i]).fileName();
        QString text = tr("&%1 %2").arg(i + 1).arg( stripped);
        recently_opened_[i]->setText( text);
        recently_opened_[i]->setData( files[i]);
        recently_opened_[i]->setVisible( true);
    }

    for( int j = num_recent_files; j < max_recently_opened_files; ++j)
        recently_opened_[j]->setVisible( false);
}

void main_window_t::init_recent_files_menu()
{
    QSettings settings( "com.ramen.qt", "Ramen Recent Files");
    QStringList files = settings.value( "recent_file_list").toStringList();

    int num_recent_files = std::min( files.size(), (int) max_recently_opened_files);

    for( int i = 0; i < num_recent_files; ++i)
    {
        QString stripped = QFileInfo( files[i]).fileName();
        QString text = tr("&%1 %2").arg(i + 1).arg( stripped);
        recently_opened_[i]->setText( text);
        recently_opened_[i]->setData( files[i]);
        recently_opened_[i]->setVisible( true);
    }

    for( int j = num_recent_files; j < max_recently_opened_files; ++j)
        recently_opened_[j]->setVisible( false);
}

void main_window_t::update_menus()
{
    /*
    bool any_selected = app().document().world_node().any_selected();
    node_t *n = app().document().world_node().selected_node();
    */

    save_->setEnabled( app().document().dirty());

    if( app().document().undo_stack().undo_empty())
    {
        undo_->setText( "Undo");
        undo_->setEnabled( false);
    }
    else
    {
        undo_->setText( QString( "Undo ") + app().document().undo_stack().last_undo_command().name().c_str());
        undo_->setEnabled( true);
    }

    if( app().document().undo_stack().redo_empty())
    {
        redo_->setText( "Redo");
        redo_->setEnabled( false);
    }
    else
    {
        redo_->setText( QString( "Redo ") + app().document().undo_stack().last_redo_command().name().c_str());
        redo_->setEnabled( true);
    }

    /*
    export_sel_->setEnabled( any_selected);

    if( n)
        comp_flipbook_->setEnabled( true);
    else
        comp_flipbook_->setEnabled( false);
    */
}

} // ui
} // ramen
