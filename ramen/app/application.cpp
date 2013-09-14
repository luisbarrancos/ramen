// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#include<ramen/app/application.hpp>

#include<cstdlib>
#include<cstring>

#include<exception>
#include<algorithm>
#include<iostream>
#include<fstream>

#include<boost/thread.hpp>
#include<boost/filesystem/operations.hpp>
#include<boost/filesystem/convenience.hpp>
#include<boost/lexical_cast.hpp>

#include<OpenEXR/ImfThreading.h>

#include<QApplication>

#include<ramen/version.hpp>

#include<ramen/system/system.hpp>

#include<ramen/app/command_line_parser.hpp>
#include<ramen/app/preferences.hpp>
#include<ramen/app/document.hpp>

#include<ramen/memory/manager.hpp>

#include<ramen/nodes/node_factory.hpp>

#include<ramen/ocio/manager.hpp>

#include<ramen/depgraph/dgraph.hpp>

#include<ramen/undo/stack.hpp>

#include<ramen/ui/user_interface.hpp>
#include<ramen/ui/main_window.hpp>
#include<ramen/ui/dialogs/splash_screen.hpp>

namespace ramen
{
namespace
{

application_t *g_app = 0;

} // unnamed

application_t::application_t( int argc, char **argv) : system_(), preferences_()
{
    RAMEN_ASSERT( g_app == 0);
    g_app = this;

    max_threads_ = 0;
    mem_cache_size_ = 0;
    quitting_ = false;

    system_.reset( new system::system_t());
    cmd_parser_.reset( new command_line_parser_t( argc, argv));

    // Create QApplication
    QApplication *q_app = new QApplication( cmd_parser_->argc, cmd_parser_->argv);
    //TODO: re-eanble this...
    //boost::filesystem::path bundle_path( system().app_bundle_path());
    //bundle_path /= "lib/Qt_plugins";
    //qApp->setLibraryPaths( QStringList( QString( ramen::filesystem::file_cstring( bundle_path))));

    cmd_parser_->parse();

    // create the application user path, if needed
    boost::filesystem::create_directories( system().application_user_path());

    // init prefs
    preferences_.reset( new preferences_t());

    if( max_threads_ == 0)
        max_threads_ = boost::thread::hardware_concurrency();

    task_scheduler_.initialize( max_threads_);
    Imf::setGlobalThreadCount( max_threads_);

    if( !run_command_line())
    {
        splash_.reset( new ui::splash_screen_t());
        splash_->show();
        splash_->show_message( RAMEN_NAME_VERSION_STR);
    }

    // init memory manager
    if( mem_cache_size_ == 0)
    {
        boost::uint64_t percent  = preferences().max_image_memory();
        boost::uint64_t ram_size = system().ram_size();
        mem_cache_size_ = ram_size / (boost::uint64_t) 100 * percent;
    }

    mem_manager_.reset( new memory::manager_t( mem_cache_size_));

    if( !run_command_line())
        splash_->show_message( "Initializing builtin nodes");
    nodes::node_factory_t::instance();

    if( !run_command_line())
        splash_->show_message( "Initializing OpenColorIO");
    ocio_manager_.reset( new ocio::manager_t());

    if( !run_command_line())
        splash_->show_message( "Initializing dependency graph");
    dependency_graph_.reset( new depgraph::dgraph_t());

    if( !run_command_line())
    {
        splash_->show_message( "Initializing user interface");
        ui_.reset( new ui::user_interface_t());
        ui_->init();
        print_app_info();
    }
}

application_t::~application_t() {}

int application_t::run()
{
    ui()->show();
    splash_->finish( ui()->main_window());
    splash_.reset();
    return ui()->run( boost::filesystem::path());
}

const system::system_t& application_t::system() const
{
    RAMEN_ASSERT( system_.get());

    return *system_;
}

const preferences_t& application_t::preferences() const    { return *preferences_;}
preferences_t& application_t::preferences()                { return *preferences_;}

const memory::manager_t& application_t::memory_manager() const { return *mem_manager_;}
memory::manager_t& application_t::memory_manager()             { return *mem_manager_;}

const ocio::manager_t& application_t::ocio_manager() const { return *ocio_manager_;}
ocio::manager_t& application_t::ocio_manager()             { return *ocio_manager_;}

const depgraph::dgraph_t& application_t::dependency_graph() const  { return *dependency_graph_;}
depgraph::dgraph_t& application_t::dependency_graph()              { return *dependency_graph_;}

const ui::user_interface_t *application_t::ui() const  { return ui_.get();}
ui::user_interface_t *application_t::ui()              { return ui_.get();}

const document_t& application_t::document() const  { return *document_;}
document_t& application_t::document()              { return *document_;}

void application_t::print_app_info()
{
    std::cout << RAMEN_NAME_FULL_VERSION_STR << ", " << __DATE__ << std::endl;
    std::cout << "System = " << system().system_name() << std::endl;
    std::cout << "Executable = " << system().executable_path().c_str() << std::endl;
    std::cout << "App dir = " << system().application_path().c_str() << std::endl;
    std::cout << "App user dir = " << system().application_user_path().c_str() << std::endl;
    std::cout << "Using " << max_threads_ << " threads" << std::endl;
    std::cout << "Ram Size = " << system().ram_size() / 1024 / 1024 << " Mb" << std::endl;
    //std::cout << "Image Cache Memory = " << mem_manager_->image_allocator().max_size() / 1024 / 1024 << " Mb" << std::endl;
}

// document handling
void application_t::create_new_document()
{
    delete_document();
    document_.reset( new document_t());
}

void application_t::open_document( const boost::filesystem::path& p)
{
    /*
    create_new_document();
    boost::filesystem::ifstream ifs( p, serialization::yaml_iarchive_t::file_open_mode());

    if( !ifs.is_open() || !ifs.good())
        throw std::runtime_error( core::string8_t( "Couldn't open input file ") + filesystem::file_string( p));

    core::auto_ptr_t<serialization::yaml_iarchive_t> in;
    in.reset( new serialization::yaml_iarchive_t( ifs));

    if( !in->read_composition_header())
        throw std::runtime_error( core::string8_t( "Couldn't read file header ") + filesystem::file_string( p));

    document().set_file( p);
    document().load( *in);

    core::string8_t err = in->errors();

    if( !err.empty())
    {
        // TODO: display errors here
        // multiline_alert_t::Instance().show_alert( "Errors during file open", err);
    }
    */
}

void application_t::delete_document()
{
    document_.reset( 0);
    //memory_manager().clear_caches();
}

// messages
void application_t::fatal_error( const core::string8_t& message, bool no_gui) const
{
    if( !run_command_line() && ui() && !ui()->rendering() && !no_gui)
        ui()->fatal_error( message);
    else
        std::cerr << "Fatal error: " << message << "\n";

    std::exit( EXIT_FAILURE);
}

void application_t::error( const core::string8_t& message, bool no_gui) const
{
    if( !run_command_line() && ui() && !ui()->rendering() && !no_gui)
        ui()->error( message);
    else
    {
        std::cerr << "Error: " << message << "\n";
    }
}

void application_t::inform( const core::string8_t& message, bool no_gui) const
{
    if( !run_command_line() && ui() && !ui()->rendering() && !no_gui)
        ui()->inform( message);
    else
        std::cerr << "Info: " << message << "\n";
}

bool application_t::question( const core::string8_t& what, bool default_answer) const
{
    if( !run_command_line() && ui() && !ui()->rendering())
        return ui()->question( what, default_answer);
    else
    {
        if( default_answer)
            std::cout << "Ramen, question: " << what << ", replying yes by default\n";
        else
            std::cout << "Ramen, question: " << what << ", replying no by default\n";
    }

    return default_answer;
}

application_t& app()
{
    RAMEN_ASSERT( g_app);
    return *g_app;
}

} // ramen
