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

TEST(HCBYTE_UNITTEST, toHexStringWithDelimiterTest) {
    ASSERT_EQ("68|65|6C|6C|6F",
              hcbyte::toHexStringWithDelimiter({'h', 'e', 'l','l', 'o'}, "|"));
}

TEST(HCBYTE_UNITTEST, toHexStringTest) {
    ASSERT_EQ("68656C6C6F",
              hcbyte::toHexString({'h', 'e', 'l','l', 'o'}));
}

TEST(HCBYTE_UNITTEST, toHexStringWithSpaceTest) {
    ASSERT_EQ("68 65 6C 6C 6F",
              hcbyte::toHexStringWithSpace({'h', 'e', 'l','l', 'o'}));
}

TEST(HCBYTE_UNITTEST, toHexStringForPrintTest) {
    ASSERT_EQ("[68, 65, 6C, 6C, 6F]",
              hcbyte::toHexStringForPrint({'h', 'e', 'l','l', 'o'}));
}

TEST(HCBYTE_UNITTEST, hexStringToBytesTest) {
    const vector<byte_t> bb = {'h', 'e', 'l','l', 'o'};
    const string hexString = hcbyte::toHexString(bb);
    const vector<byte_t> expectBytes = {0x68, 0x65, 0x6C, 0x6C, 0x6F};
    ASSERT_EQ(expectBytes,hcbyte::hexStringToBytes(hexString));

    const string hexString2 = "12 34";
    const vector<byte_t> expectBytes2 = {0x12, 0x34};
    ASSERT_EQ(expectBytes2,hcbyte::hexStringToBytes(hexString2));
}

TEST(HCBYTE_UNITTEST, to4ByteArrayTest) {
    const vector<byte_t> expectBytes = {0x00, 0x00, 0x00, 0x0A};
    const vector<byte_t> bb = hcbyte::to4ByteArray(10);

    ASSERT_EQ(bb, expectBytes);
}

TEST(HCBYTE_UNITTEST, from4ByteArrayTest) {
    const int expectResult = 10;
    const vector<byte_t> bb = {0x00, 0x00, 0x00, 0x0A};
    const int result = hcbyte::from4ByteArray(bb);

    ASSERT_EQ(result, expectResult);

    const vector<byte_t> bb2 = {0x00, 0x0A};
    ASSERT_THROW(hcbyte::from4ByteArray(bb2), happycpp::HappyException);
}

TEST(HCBYTE_UNITTEST, to2ByteArrayTest) {
    const vector<byte_t> expectBytes = {0x00, 0x0A};
    const vector<byte_t> bb = hcbyte::to2ByteArray( 10U);
    ASSERT_EQ(bb, expectBytes);
}

TEST(HCBYTE_UNITTEST, from2ByteArrayTest) {
    const int expectResult = 10;
    const vector<byte_t> bb = {0x00, 0x0A};
    const int result = hcbyte::from2ByteArray(bb);

    ASSERT_EQ(result, expectResult);

    const vector<byte_t> bb2 = {0x00};
    ASSERT_THROW(hcbyte::from2ByteArray(bb2), happycpp::HappyException);
}


int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}