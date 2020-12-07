// -*- C++ -*-
// Copyright (c) 2016, Fifi Lyu. All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

/** @file */

#ifndef INCLUDE_HAPPYCPP_COMMON_H_
#define INCLUDE_HAPPYCPP_COMMON_H_

#include <cassert>
#include "happycpp/config_compiler.h"
#include "happycpp/config_platform.h"
#include "happycpp/type.h"
#include <boost/filesystem.hpp>

#ifdef PLATFORM_WIN32
#define OsSeparator "\\"
#else
#define OsSeparator "/"
#endif

namespace bfs = boost::filesystem;

namespace happycpp {

#ifdef _DEBUG
#define HAPPY_ASSERT(expression) assert(expression);
#else
#define HAPPY_ASSERT(expression) {\
  if ((expression) == 0) {\
    fprintf(stderr, "[Error] Fatal error,force quit.\n");\
    exit(EXIT_FAILURE);\
  }\
}
#endif

#ifdef PLATFORM_WIN32
    // End Of Line
#define EOL "\r\n"
#else
// End Of Line
#define EOL "\n"
#endif

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_COMMON_H_
