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
#include "happycpp/algorithm/byte.h"
#include "happycpp/exception.h"

using namespace std;
namespace hcbyte = happycpp::hcalgorithm::hcbyte;

TEST(HCBYTE_UNITTEST, ToHexStringWithDelimiter) { // NOLINT
    EXPECT_EQ("68|65|6C|6C|6F",
              hcbyte::toHexStringWithDelimiter({'h', 'e', 'l', 'l', 'o'}, "|"));
}

TEST(HCBYTE_UNITTEST, ToHexString) { // NOLINT
    EXPECT_EQ("68656C6C6F",
              hcbyte::toHexString({'h', 'e', 'l', 'l', 'o'}));
}

TEST(HCBYTE_UNITTEST, toHexStringWithSpace) { // NOLINT
    EXPECT_EQ("68 65 6C 6C 6F",
              hcbyte::toHexStringWithSpace({'h', 'e', 'l', 'l', 'o'}));
}

TEST(HCBYTE_UNITTEST, ToHexStringForPrint) { // NOLINT
    EXPECT_EQ("[68, 65, 6C, 6C, 6F]",
              hcbyte::toHexStringForPrint({'h', 'e', 'l', 'l', 'o'}));
}

TEST(HCBYTE_UNITTEST, HexStringToBytes) { // NOLINT
    const vector<byte_t> bb = {'h', 'e', 'l', 'l', 'o'};
    const string hexString = hcbyte::toHexString(bb);
    const vector<byte_t> expectBytes = {0x68, 0x65, 0x6C, 0x6C, 0x6F};
    EXPECT_EQ(expectBytes, hcbyte::hexStringToBytes(hexString));

    const string hexString2 = "12 34";
    const vector<byte_t> expectBytes2 = {0x12, 0x34};
    EXPECT_EQ(expectBytes2, hcbyte::hexStringToBytes(hexString2, " "));
}

TEST(HCBYTE_UNITTEST, To4ByteArray) { // NOLINT
    const vector<byte_t> expectBytes = {0x00, 0x00, 0x00, 0x0A};
    const vector<byte_t> bb = hcbyte::to4ByteArray(10);

    EXPECT_EQ(bb, expectBytes);
}

TEST(HCBYTE_UNITTEST, From4ByteArray) { // NOLINT
    const int expectResult = 10;
    const vector<byte_t> bb = {0x00, 0x00, 0x00, 0x0A};
    const int result = hcbyte::from4ByteArray(bb);

    EXPECT_EQ(result, expectResult);

    const vector<byte_t> bb2 = {0x00, 0x0A};
    EXPECT_THROW(hcbyte::from4ByteArray(bb2), happycpp::HappyException);
}

TEST(HCBYTE_UNITTEST, To2ByteArray) { // NOLINT
    const vector<byte_t> expectBytes = {0x00, 0x0A};
    const vector<byte_t> bb = hcbyte::to2ByteArray(10U);
    EXPECT_EQ(bb, expectBytes);
}

TEST(HCBYTE_UNITTEST, From2ByteArray) { // NOLINT
    const int expectResult = 10;
    const vector<byte_t> bb = {0x00, 0x0A};
    const int result = hcbyte::from2ByteArray(bb);

    EXPECT_EQ(result, expectResult);

    const vector<byte_t> bb2 = {0x00};
    EXPECT_THROW(hcbyte::from2ByteArray(bb2), happycpp::HappyException);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
