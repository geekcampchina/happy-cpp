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
#include "happycpp/iconv.h"

using namespace std;
using namespace happycpp::hciconv;

TEST(HCICONV_UNITTEST, IconvConvertTestForGB2312ToUTF8) { // NOLINT
    const string s = "GB2312 \xC4\xE3\xBA\xC3\xA3\xAC\xCA\xC0\xBD\xE7\xA3\xA1";
    const string expectedResult = "GB2312 你好，世界！";
    const string result = iconvConvert(StandardCharsets::GB2312, StandardCharsets::UTF8, s);
    EXPECT_EQ(expectedResult, result);
}

TEST(HCICONV_UNITTEST, IconvConvertTestForUTF8ToGB2312) { // NOLINT
    const string s = "UTF-8 你好，世界！";
    const string expectedResult = "UTF-8 \xC4\xE3\xBA\xC3\xA3\xAC\xCA\xC0\xBD\xE7\xA3\xA1";
    const string result = iconvConvert(StandardCharsets::UTF8, StandardCharsets::GB2312, s);
    EXPECT_EQ(expectedResult, result);
}

TEST(HCICONV_UNITTEST, IconvConvertTestForGBKToUTF8) { // NOLINT
    const string s = "GBK \xC4\xE3\xBA\xC3\xA3\xAC\xCA\xC0\xBD\xE7\xA3\xA1";
    const string expectedResult = "GBK 你好，世界！";
    const string result = iconvConvert(StandardCharsets::GBK, StandardCharsets::UTF8, s);
    EXPECT_EQ(expectedResult, result);
}

TEST(HCICONV_UNITTEST, IconvConvertTestForUTF8ToGBK) { // NOLINT
    const string s = "UTF-8 你好，世界！";
    const string expectedResult = "UTF-8 \xC4\xE3\xBA\xC3\xA3\xAC\xCA\xC0\xBD\xE7\xA3\xA1";
    const string result = iconvConvert(StandardCharsets::UTF8, StandardCharsets::GBK, s);
    EXPECT_EQ(expectedResult, result);
}

TEST(HCICONV_UNITTEST, IconvConvertTestForGB18030ToUTF8) { // NOLINT
    const string s = "GB18030 \xC4\xE3\xBA\xC3\xA3\xAC\xCA\xC0\xBD\xE7\xA3\xA1";
    const string expectedResult = "GB18030 你好，世界！";
    const string result = iconvConvert(StandardCharsets::GB18030, StandardCharsets::UTF8, s);
    EXPECT_EQ(expectedResult, result);
}

TEST(HCICONV_UNITTEST, IconvConvertTestForUTF8ToGB18030) { // NOLINT
    const string s = "UTF-8 你好，世界！";
    const string expectedResult = "UTF-8 \xC4\xE3\xBA\xC3\xA3\xAC\xCA\xC0\xBD\xE7\xA3\xA1";
    const string result = iconvConvert(StandardCharsets::UTF8, StandardCharsets::GB18030, s);
    EXPECT_EQ(expectedResult, result);
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
