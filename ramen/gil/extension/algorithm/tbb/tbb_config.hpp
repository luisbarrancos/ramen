//  Copyright Esteban Tovagliari 2008. Use, modification and
//  distribution is subject to the Boost Software License, Version
//  1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#include <tbb/partitioner.h>
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>

#ifndef GIL_TBB_ALGORITHMS_DEFAULT_PARTITIONER
#define GIL_TBB_ALGORITHMS_DEFAULT_PARTITIONER tbb::auto_partitioner
#endif
