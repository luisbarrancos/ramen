// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_UI_MAIN_WINDOW_HPP
#define	RAMEN_UI_MAIN_WINDOW_HPP

#include<ramen/ui/main_window_fwd.hpp>

#include<QMainWindow>
#include<QDockWidget>

#include<boost/filesystem/path.hpp>

#include<ramen/core/memory.hpp>
#include<ramen/core/name.hpp>

#include<ramen/qwidgets/time_slider_fwd.hpp>

#include<ramen/ui/anim/anim_editor_fwd.hpp>
#include<ramen/ui/inspector/inspector_fwd.hpp>
#include<ramen/ui/nodes_view/nodes_view_tabs_fwd.hpp>
#include<ramen/ui/time_controls_fwd.hpp>
#include<ramen/ui/viewer/viewer_tabs_fwd.hpp>

class QAction;
class QMenu;
class QMenuBar;
class QDockWidget;
class QToolBar;

namespace ramen
{
namespace ui
{

/**
\ingroup ui
\brief main window class.
*/
class main_window_t : public QMainWindow
{
    Q_OBJECT

public:

    main_window_t();

    void add_dock_widget( Qt::DockWidgetArea area, QDockWidget *dock);

    const ui::nodes_view_tabs_t& nodes_view_tabs() const  { return *nodes_view_tabs_;}
    ui::nodes_view_tabs_t& nodes_view_tabs()              { return *nodes_view_tabs_;}

    const inspector_t& inspector() const    { return *inspector_;}
    inspector_t& inspector()                { return *inspector_;}

    const anim_editor_t& anim_editor() const { return *anim_editor_;}
    anim_editor_t& anim_editor()             { return *anim_editor_;}

    const qwidgets::time_slider_t& time_slider() const    { return *time_slider_;}
    qwidgets::time_slider_t& time_slider()                { return *time_slider_;}

    const time_controls_t& time_controls() const    { return *time_controls_;}
    time_controls_t& time_controls()                { return *time_controls_;}

    void update_state();

    bool can_close_document();

    void update_recent_files_menu( const boost::filesystem::path& p);

protected:

    void closeEvent( QCloseEvent *event);

public Q_SLOTS:

    void new_document();
    void open_document();
    void open_recent_document();

    void save_document();
    void save_document_as();

    void quit();

    void undo();
    void redo();

    void show_preferences_dialog();

    void create_viewer();

    void show_about_box();
    void go_to_project_website();

private:

    QToolBar *create_time_toolbar();

    void create_actions();
    void create_menus();

    void init_recent_files_menu();
    void update_menus();

    static const char *document_extension();
    static const char *file_dialog_extension();

    qwidgets::time_slider_t *time_slider_;
    nodes_view_tabs_t *nodes_view_tabs_;
    viewer_tabs_t *viewer_tabs_;
    time_controls_t *time_controls_;
    QDockWidget *inspector_dock_;
    inspector_t *inspector_;
    QDockWidget *node_graph_dock_;
    QDockWidget *viewer_dock_;
    QDockWidget *anim_editor_dock_;
    anim_editor_t *anim_editor_;

    QMenuBar *menubar_;

    QMenu *file_, *edit_, *view_, *help_;
    QMenu *open_recent_;
    QMenu *import_, *export_;

    std::vector<QAction *> recently_opened_;

    // file
    QAction *new_, *open_, *save_, *save_as_;
    QAction *quit_;

    // edit
    QAction *undo_;
    QAction *redo_;
    QAction *preferences_;

    // window
    QAction *create_viewer_;

    // help
    QAction *about_, *project_web_;

    // non - menu actions
    QAction *next_frame_, *prev_frame_;

    const static int max_recently_opened_files;
};

} // ui
} // ramen

#endif

