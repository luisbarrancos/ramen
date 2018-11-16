// Copyright (c) 2013 Esteban Tovagliari
// Licensed under the terms of the CDDL License.
// See CDDL_LICENSE.txt for a copy of the license.

#pragma once

#if defined(linux) || defined(__linux)
#define RAMEN_CONFIG_OS_LINUX
#elif defined(__APPLE__) && defined(__MACH__)
#define RAMEN_CONFIG_OS_OSX
#elif defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || \
    defined(__TOS_WIN__) || defined(__WINDOWS__)
#define RAMEN_CONFIG_OS_WINDOWS
#endif
