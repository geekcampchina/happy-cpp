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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_TIME_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_TIME_H_

#include <happycpp/common.h>
#include <string>
#include <ctime>

namespace happycpp {

namespace hcalgorithm {

namespace hctime {
// 延迟指定的时间，单位:秒
HAPPYCPP_SHARED_LIB_API void HappyWait(const time_t &sec);

// 延迟指定的时间，单位:毫秒
HAPPYCPP_SHARED_LIB_API void HappySleep(const time_t &milli_sec);

/*
 比较格式相同的日期字符串
 第一个日期比第二个日期早，返回值小于0
 第一个日期和第二个日期相等，返回值等于0
 第一个日期比第二个日期晚，返回值大于0
 */
HAPPYCPP_SHARED_LIB_API CmpResult CmpDate(const std::string &d1,
                                          const std::string &d2);

/**
 * @brief 按照制定样式格式化时间
 * @param t 时间
 * @param format 样式
 * @return 字符串
 */
std::string GetFormatTime(const time_t t, const std::string &format);

} /* namespace hctime */

} /* namespace hcalgorithm */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_TIME_H_
