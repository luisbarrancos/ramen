// Copyright (c) 2012 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#ifndef RAMEN_APP_COMMAND_LINE_PARSER_HPP
#define RAMEN_APP_COMMAND_LINE_PARSER_HPP

#include<ramen/app/command_line_parser_fwd.hpp>

namespace ramen
{

class command_line_parser_t
{
public:

    command_line_parser_t( int cmd_argc, char **cmd_argv);
    ~command_line_parser_t();

    void parse();

    int argc;
    char **argv;

private:

    void usage();
};

} // ramen

#endif
