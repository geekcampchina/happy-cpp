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
#include "happycpp/algorithm/unit.h"

namespace hhhunit = happycpp::hcalgorithm::hcunit;

TEST(HCUNIT_UNITTEST, ToStr) {
    EXPECT_STREQ("byte", hhhunit::toStr(kUnitByte).c_str());
    EXPECT_STREQ("KiB", hhhunit::toStr(kUnitKiB).c_str());
    EXPECT_STREQ("MiB", hhhunit::toStr(kUnitMiB).c_str());
    EXPECT_STREQ("GiB", hhhunit::toStr(kUnitGiB).c_str());
    EXPECT_STREQ("TiB", hhhunit::toStr(kUnitTiB).c_str());
    EXPECT_STREQ("PiB", hhhunit::toStr(kUnitPiB).c_str());
    EXPECT_STREQ("KB", hhhunit::toStr(kUnitKB).c_str());
    EXPECT_STREQ("MB", hhhunit::toStr(kUnitMB).c_str());
    EXPECT_STREQ("GB", hhhunit::toStr(kUnitGB).c_str());
    EXPECT_STREQ("TB", hhhunit::toStr(kUnitTB).c_str());
    EXPECT_STREQ("PB", hhhunit::toStr(kUnitPB).c_str());
}

TEST(HCUNIT_UNITTEST, ToByte1) {
    EXPECT_EQ(1UL, hhhunit::toByte(kUnitByte));

    EXPECT_EQ(1024UL, hhhunit::toByte(kUnitKiB));
    EXPECT_EQ(1048576UL, hhhunit::toByte(kUnitMiB));
    EXPECT_EQ(1073741824UL, hhhunit::toByte(kUnitGiB));
    EXPECT_EQ(1099511627776UL, hhhunit::toByte(kUnitTiB));
    EXPECT_EQ(1125899906842624UL, hhhunit::toByte(kUnitPiB));

    EXPECT_EQ(1000UL, hhhunit::toByte(kUnitKB));
    EXPECT_EQ(1000000UL, hhhunit::toByte(kUnitMB));
    EXPECT_EQ(1000000000UL, hhhunit::toByte(kUnitGB));
    EXPECT_EQ(1000000000000UL, hhhunit::toByte(kUnitTB));
    EXPECT_EQ(1000000000000000UL, hhhunit::toByte(kUnitPB));
}

TEST(HCUNIT_UNITTEST, ToByte2) {
    const ssize_t s = 3;

    EXPECT_EQ(3U, hhhunit::toByte(kUnitByte, s));
}

TEST(HCUNIT_UNITTEST, Convert) {
    // 1024 PiB
    const byte_t b1 = 1152921504606846976UL;

    EXPECT_DOUBLE_EQ(1152921504606846976UL, hhhunit::convert(kUnitByte, b1, 0));

    EXPECT_DOUBLE_EQ(1125899906842624.0, hhhunit::convert(kUnitKiB, b1, 0));
    EXPECT_DOUBLE_EQ(1099511627776.0, hhhunit::convert(kUnitMiB, b1, 0));
    EXPECT_DOUBLE_EQ(1073741824.0, hhhunit::convert(kUnitGiB, b1, 0));
    EXPECT_DOUBLE_EQ(1048576.0, hhhunit::convert(kUnitTiB, b1, 0));
    EXPECT_DOUBLE_EQ(1024.0, hhhunit::convert(kUnitPiB, b1, 0));

    EXPECT_DOUBLE_EQ(1152921504606847.00, hhhunit::convert(kUnitKB, b1, 2));
    EXPECT_DOUBLE_EQ(1152921504606.85, hhhunit::convert(kUnitMB, b1, 2));
    EXPECT_DOUBLE_EQ(1152921504.61, hhhunit::convert(kUnitGB, b1, 2));
    EXPECT_DOUBLE_EQ(1152921.50, hhhunit::convert(kUnitTB, b1, 2));
    EXPECT_DOUBLE_EQ(1152.92, hhhunit::convert(kUnitPB, b1, 2));

    // 1000 PB
    const byte_t b2 = 1000000000000000000UL;

    EXPECT_DOUBLE_EQ(1000000000000000000UL, hhhunit::convert(kUnitByte, b2, 0));

    EXPECT_DOUBLE_EQ(976562500000000.00, hhhunit::convert(kUnitKiB, b2, 2));
    EXPECT_DOUBLE_EQ(953674316406.25, hhhunit::convert(kUnitMiB, b2, 2));
    EXPECT_DOUBLE_EQ(931322574.62, hhhunit::convert(kUnitGiB, b2, 2));
    EXPECT_DOUBLE_EQ(909494.70, hhhunit::convert(kUnitTiB, b2, 2));
    EXPECT_DOUBLE_EQ(888.18, hhhunit::convert(kUnitPiB, b2, 2));

    EXPECT_DOUBLE_EQ(1000000000000000.0, hhhunit::convert(kUnitKB, b2, 0));
    EXPECT_DOUBLE_EQ(1000000000000.0, hhhunit::convert(kUnitMB, b2, 0));
    EXPECT_DOUBLE_EQ(1000000000.0, hhhunit::convert(kUnitGB, b2, 0));
    EXPECT_DOUBLE_EQ(1000000.0, hhhunit::convert(kUnitTB, b2, 0));
    EXPECT_DOUBLE_EQ(1000.0, hhhunit::convert(kUnitPB, b2, 0));
}
