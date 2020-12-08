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
#include "happycpp/filesys.h"
#include "happycpp/hcerrno.h"
#include "happycpp/exception.h"
#include <iostream>

#ifdef PLATFORM_WIN32
#ifndef INCLUDE_WINDOWS_H_FILE
#define INCLUDE_WINDOWS_H_FILE
#include <Windows.h>
#endif
#else

#include <errno.h>

#endif

TEST(HCERRNO_UNITTEST, ErrorToStr) {
#ifdef PLATFORM_WIN32
    SetLastError(0);
    EXPECT_STREQ("", happycpp::hcerrno::ErrorToStr().c_str());

    std::ifstream ifs("test_+abc.txt");
    EXPECT_STREQ("The system cannot find the file specified(errno: 2)",
                 happycpp::hcerrno::ErrorToStr().c_str());
#else
    errno = 0;
    EXPECT_STREQ("", happycpp::hcerrno::errorToStr().c_str());

    std::ifstream ifs("test_+abc.txt");
    EXPECT_STREQ("No such file or directory(errno: 2)",
                 happycpp::hcerrno::errorToStr().c_str());
#endif
}
