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

/** @file
 *  无任何非系统库依赖，功能简单的日志系统
 */

#ifndef INCLUDE_HAPPYCPP_LOG_H_
#define INCLUDE_HAPPYCPP_LOG_H_

#include <happycpp/common.h>

namespace happycpp {

namespace hclog {

extern HAPPYCPP_SHARED_LIB_API LogConfig G_LOG_CONFIG;

HAPPYCPP_SHARED_LIB_API void _PrintLog(LogLevelNumber level, const char *file,
                                       int line, const char *fmt, ...);

int to_log_level_num(const std::string &level_str);

std::string to_log_level_str(const int level_num);

} /* namespace hclog */

} /* namespace happycpp */

#define PrintLog(level, ...) \
  happycpp::hclog::_PrintLog(level, __FILE__, __LINE__, __VA_ARGS__)

#endif  // INCLUDE_HAPPYCPP_LOG_H_
