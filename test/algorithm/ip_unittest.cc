﻿// Copyright (c) 2016, Fifi Lyu. All rights reserved.
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
#include "happycpp/algorithm/ip.h"

namespace hhhip = happycpp::hcalgorithm::hcip;

TEST(HCIP_UNITTEST, IsIpAddr) { // NOLINT
    EXPECT_TRUE(hhhip::isIpAddr("1.1.1.1"));
    EXPECT_FALSE(hhhip::isIpAddr("1.1.1.266"));
    EXPECT_FALSE(hhhip::isIpAddr("abc"));
}

TEST(HCIP_UNITTEST, IsReserveIpAddr) { // NOLINT
    EXPECT_TRUE(hhhip::isReserveIpAddr("0.0.0.0"));
    EXPECT_TRUE(hhhip::isReserveIpAddr("192.168.1.1"));
    EXPECT_TRUE(hhhip::isReserveIpAddr("255.255.255.255"));
    EXPECT_FALSE(hhhip::isReserveIpAddr("8.8.8.8"));
    EXPECT_FALSE(hhhip::isReserveIpAddr("abc"));
}

TEST(HCIP_UNITTEST, IsIpCidr) { // NOLINT
    EXPECT_FALSE(hhhip::isIpCidr("1.1.1.1"));
    EXPECT_TRUE(hhhip::isIpCidr("1.1.1.1/8"));
    EXPECT_TRUE(hhhip::isIpCidr("1.1.1.1/32"));
    EXPECT_FALSE(hhhip::isIpCidr("1.1.1.1/33"));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
