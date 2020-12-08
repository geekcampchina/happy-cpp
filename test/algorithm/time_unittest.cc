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

#include <gtest/gtest.h>
#include "happycpp/algorithm/hctime.h"
#include "happycpp/algorithm/hcstring.h"
#include <sys/timeb.h>

namespace hhhtime = happycpp::hcalgorithm::hctime;

TEST(HCTIME_UNITTEST, HappyWait) {
    const time_t sec = 1;
    time_t start;
    time_t end;

    time(&start);
    hhhtime::happyWait(sec);
    time(&end);

    const double diff = difftime(end, start);

    // diff >= sec
    EXPECT_GE(diff, sec);
}

TEST(HCTIME_UNITTEST, HappySleep) {
#ifdef PLATFORM_LINUX
    typedef struct timeb _timeb;
#define _ftime(timebuf) ftime(timebuf);
#endif

    const time_t milli_sec = 2100;
    _timeb start;
    _timeb end;

    _ftime(&start);
    hhhtime::happySleep(milli_sec);
    _ftime(&end);

    const time_t diff =
            (1000 * (end.time - start.time) + (end.millitm - start.millitm));

    // diff >= 1000
    EXPECT_GE(diff, static_cast<time_t>(1200));
}

TEST(HCTIME_UNITTEST, CmpDate) {
    CmpResult ret = hhhtime::cmpDate("2015-11-16 14:06:36",
                                     "2015-04-09 00:11:36");
    EXPECT_EQ(kGreater, ret);
}

TEST(HCTIME_UNITTEST, GetFormatTime) {
    ASSERT_EQ("20160325", hhhtime::getFormatTime(1458893661, "%Y%m%d"));
}

