// Copyright (c) 2010 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_APP_APPLICATION_HPP
#define RAMEN_APP_APPLICATION_HPP

#include<ramen/config.hpp>

#include<ramen/app/application_fwd.hpp>

#include<memory>

#include<boost/cstdint.hpp>
#include<boost/optional.hpp>
#include<boost/filesystem/path.hpp>

#include<tbb/task_scheduler_init.h>

#include<ramen/core/memory.hpp>
#include<ramen/core/string8.hpp>

#include<ramen/system/system_fwd.hpp>

#include<ramen/app/command_line_parser_fwd.hpp>
#include<ramen/app/preferences.hpp>
#include<ramen/app/document_fwd.hpp>

#include<ramen/memory/manager_fwd.hpp>
#include<ramen/ocio/manager_fwd.hpp>

#include<ramen/depgraph/dgraph_fwd.hpp>

#include<ramen/ui/user_interface_fwd.hpp>
#include<ramen/ui/dialogs/splash_screen_fwd.hpp>

namespace ramen
{

/*!
\ingroup app
\brief Application class.
*/
class RAMEN_API application_t
{
public:

    application_t( int argc, char **argv);
    ~application_t();

    int run();

    // messages
    void fatal_error( const core::string8_t& message, bool no_gui = false) const;
    void error( const core::string8_t& message, bool no_gui = false) const;
    void inform( const core::string8_t& message, bool no_gui = false) const;
    bool question( const core::string8_t& what, bool default_answer = true) const;

    // system info
    const system::system_t& system() const;

    // preferences
    const preferences_t& preferences() const;
    preferences_t& preferences();

    // memmory manager
    const memory::manager_t& memory_manager() const;
    memory::manager_t& memory_manager();

    // opencolorio
    const ocio::manager_t& ocio_manager() const;
    ocio::manager_t& ocio_manager();

    // dependency graph
    const depgraph::dgraph_t& dependency_graph() const;
    depgraph::dgraph_t& dependency_graph();

    // user interface
    const ui::user_interface_t *ui() const;
    ui::user_interface_t *ui();

    // document handling
    const document_t& document() const;
    document_t& document();

    void create_new_document();
    void open_document( const boost::filesystem::path& p);
    void delete_document();

    bool quitting() const       { return quitting_;}
    void set_quitting( bool b)  { quitting_ = b;}

private:

    // non-copyable
    application_t( const application_t&);
    application_t& operator=( const application_t&);

    // logging
    void print_app_info();

    // opencolorio
    void init_ocio();
    bool init_ocio_config_from_file( const boost::filesystem::path& p);

    bool run_command_line() const { return false;}

    // data
    core::auto_ptr_t<command_line_parser_t> cmd_parser_;
    boost::uint64_t mem_cache_size_;
    int max_threads_;
    bool quitting_;

    tbb::task_scheduler_init task_scheduler_;
    core::auto_ptr_t<system::system_t> system_;
    core::auto_ptr_t<preferences_t> preferences_;
    core::auto_ptr_t<memory::manager_t> mem_manager_;
    core::auto_ptr_t<ocio::manager_t> ocio_manager_;
    core::auto_ptr_t<depgraph::dgraph_t> dependency_graph_;
    core::auto_ptr_t<ui::user_interface_t> ui_;
    core::auto_ptr_t<document_t> document_;
    core::auto_ptr_t<ui::splash_screen_t> splash_;
};

RAMEN_API application_t& app();

} // ramen

#endif
