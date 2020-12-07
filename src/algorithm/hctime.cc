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

#include "happycpp/algorithm/hctime.h"
#include "happycpp/exception.h"

#ifdef PLATFORM_WIN32
#ifndef INCLUDE_WINDOWS_H_FILE
#define INCLUDE_WINDOWS_H_FILE
#include <Windows.h>
#endif
#else

#include <ctime>

#endif

#include <sstream>
#include <cstdio>

using std::stringstream;

namespace happycpp {

    namespace hcalgorithm {

        namespace hctime {
            HAPPYCPP_SHARED_LIB_API void HappyWait(const time_t &sec) {
                clock_t end_wait = clock() + sec * CLOCKS_PER_SEC;

                while (clock() < end_wait) {}
            }

            HAPPYCPP_SHARED_LIB_API void HappySleep(const time_t &milli_sec) {
#ifdef PLATFORM_WIN32
                Sleep(static_cast<DWORD>(milli_sec));
#else
                struct timespec req = {0, 0};
                auto sec = static_cast<time_t>(milli_sec / 1000);

                const time_t _milli_sec = milli_sec - (sec * 1000);
                req.tv_sec = sec;
                req.tv_nsec = _milli_sec * 1000000L;

                while (nanosleep(&req, &req) == -1)
                    continue;

#endif
            }

            HAPPYCPP_SHARED_LIB_API CmpResult CmpDate(const std::string &d1,
                                                      const std::string &d2) {
                /*
                 格式相同的字符串，可以直接比较
                 C 代码：
                 int ret = strcmp(date1, date2)
                 ret < 0，date1 < date2
                 ret == 0，date1 == date2
                 ret > 0，date1 > date2
                 */
                if (d1 < d2)
                    return kLess;
                else if (d1 == d2)
                    return kEqual;
                else
                    return kGreater;
            }

            std::string GetFormatTime(const time_t t, const std::string &format) {
                struct tm _tm = {0};
                time_t _t = t;
#ifdef PLATFORM_WIN32
                localtime_s(&_tm, &_t);
#else
                localtime_r(&_t, &_tm);
#endif

                char buffer[80];
                const int ret_val = strftime(buffer, 80, format.c_str(), &_tm);

                return ret_val == 0 ? "" : buffer;
            }

        } /* namespace hctime */

    } /* namespace hcalgorithm */

} /* namespace happycpp */
