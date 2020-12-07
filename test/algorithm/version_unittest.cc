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
#include "happycpp/algorithm/version.h"

namespace hhhversion = happycpp::hcalgorithm::hcversion;

hhhversion::Version g_version("1.2.3");

TEST(HCVERSION_UNITTEST, major) {
    ASSERT_EQ(1U, g_version.major());
}

TEST(HCVERSION_UNITTEST, minor) {
    ASSERT_EQ(2U, g_version.minor());
}

TEST(HCVERSION_UNITTEST, revision) {
    ASSERT_EQ(3U, g_version.revision());
}

TEST(HCVERSION_UNITTEST, build) {
    ASSERT_EQ(0U, g_version.build());
}

TEST(HCVERSION_UNITTEST, LT) {
    ASSERT_TRUE(hhhversion::Version("1.2.0") < g_version);
}

TEST(HCVERSION_UNITTEST, LE) {
    ASSERT_TRUE(hhhversion::Version("1.0.0") <= g_version);
    ASSERT_TRUE(hhhversion::Version("1.2.0") <= g_version);
}

TEST(HCVERSION_UNITTEST, EQ) {
    ASSERT_TRUE(hhhversion::Version("1.2.3") <= g_version);
    ASSERT_TRUE(hhhversion::Version("1.2.3.0") <= g_version);
}

TEST(HCVERSION_UNITTEST, GT) {
    ASSERT_TRUE(hhhversion::Version("1.2.3.1") > g_version);
}

TEST(HCVERSION_UNITTEST, GE) {
    ASSERT_TRUE(hhhversion::Version("1.2.3.1") >= g_version);
    ASSERT_TRUE(hhhversion::Version("1.2.3") >= g_version);
}
