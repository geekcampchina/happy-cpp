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
#include "happycpp/algorithm/hcstring.h"
#include <string>

namespace hhhstring = happycpp::hcalgorithm::hcstring;

TEST(HCSTRING_UNITTEST, Find) { // NOLINT
    EXPECT_TRUE(hhhstring::find("abcdefg", "cd"));
    EXPECT_FALSE(hhhstring::find("abcdefg", "123"));
    EXPECT_FALSE(hhhstring::find("ABC", "abcdefg"));
}

TEST(HCSTRING_UNITTEST, Trim) { // NOLINT
    EXPECT_STREQ("abc", hhhstring::trim(" abc", " ").c_str());
    EXPECT_STREQ("abc", hhhstring::trim("abc ", " ").c_str());
    EXPECT_STREQ("abc", hhhstring::trim("abc", " ").c_str());

    EXPECT_STREQ(" abc", hhhstring::trim(" abc\t", "\t").c_str());
    EXPECT_STREQ("abc ", hhhstring::trim("\tabc ", "\t").c_str());
    EXPECT_STREQ("abc\t ", hhhstring::trim("abc\t ", "\t").c_str());

    EXPECT_STREQ("abc", hhhstring::trim("abc", " \t").c_str());
    EXPECT_STREQ("abc", hhhstring::trim(" abc\t ", " \t").c_str());
    EXPECT_STREQ("abc", hhhstring::trim("\tabc ", " \t").c_str());
    EXPECT_STREQ("abc", hhhstring::trim(" \tabc \t", " \t").c_str());
}

TEST(HCSTRING_UNITTEST, ToLower) { // NOLINT
    EXPECT_STREQ("abc", hhhstring::toLower("abc").c_str());
    EXPECT_STREQ("abc", hhhstring::toLower("ABC").c_str());
}

TEST(HCSTRING_UNITTEST, ToUpper) { // NOLINT
    EXPECT_STREQ("ABC", hhhstring::toUpper("abc").c_str());
    EXPECT_STREQ("ABC", hhhstring::toUpper("ABC").c_str());
    EXPECT_STREQ(" ABC ", hhhstring::toUpper(" abc ").c_str());
}

TEST(HCSTRING_UNITTEST, Replace) { // NOLINT
    EXPECT_STREQ("1a111b1113c",
                 hhhstring::replace("1a2b23c", "2", "111").c_str());
    EXPECT_STREQ("", hhhstring::replace("", "abc", "111").c_str());
    EXPECT_STREQ("abc", hhhstring::replace("abc", "", "111").c_str());
    EXPECT_STREQ("abc", hhhstring::replace("abc", "111", "111").c_str());
    EXPECT_STREQ("1ab3c", hhhstring::replace("1a111b1113c", "111", "").c_str());
}

TEST(HCSTRING_UNITTEST, Erase) { // NOLINT
    EXPECT_STREQ("1ab3c", hhhstring::erase("1a22b223c", "2").c_str());
    EXPECT_STREQ("1a22b223c", hhhstring::erase("1a22b223c", "").c_str());
    EXPECT_STREQ("", hhhstring::erase("", "abc").c_str());
}

TEST(HCSTRING_UNITTEST, IsDigit) { // NOLINT
    EXPECT_TRUE(hhhstring::isDigit("123"));
    EXPECT_FALSE(hhhstring::isDigit("12a3"));
}

TEST(HCSTRING_UNITTEST, IsVersion) { // NOLINT
    EXPECT_TRUE(hhhstring::isVersion("1.2.3"));
    EXPECT_FALSE(hhhstring::isVersion(""));
    EXPECT_FALSE(hhhstring::isVersion("1.a.3"));
    EXPECT_FALSE(hhhstring::isVersion(".123"));
    EXPECT_FALSE(hhhstring::isVersion("123."));
}

TEST(HCSTRING_UNITTEST, IsAlnum) { // NOLINT
    EXPECT_TRUE(hhhstring::isAlnum("12a3"));
    EXPECT_TRUE(hhhstring::isAlnum("123"));
    EXPECT_FALSE(hhhstring::isAlnum(""));
    EXPECT_FALSE(hhhstring::isAlnum("12+3"));
}

TEST(HCSTRING_UNITTEST, IsAlpha) { // NOLINT
    EXPECT_TRUE(hhhstring::isAlpha("abc"));
    EXPECT_FALSE(hhhstring::isAlpha(""));
    EXPECT_FALSE(hhhstring::isAlpha("123abc"));
}

// c++11 标准函数测试
TEST(HCSTRING_UNITTEST, StringCast) { // NOLINT
    const float expected1 = std::stof("2.1");
    const float actual1 = 2.1f;
    EXPECT_DOUBLE_EQ(actual1, expected1);

    const double expected2 = std::stod("2.1111");
    const double actual2 = 2.1111;
    EXPECT_DOUBLE_EQ(actual2, expected2);

    const int16_t expected3 = std::stoi("12345");
    const int16_t actual3 = 12345;
    EXPECT_EQ(actual3, expected3);

    const int32_t expected4 = std::stoi("1234567890");
    const int32_t actual4 = 1234567890;
    EXPECT_EQ(actual4, expected4);

    const int64_t expected5 = std::stol("1234567890");
    const int64_t actual5 = 1234567890;
    EXPECT_EQ(actual5, expected5);

    const uint16_t expected6 = std::stoi("12345");
    const uint16_t actual6 = 12345U;
    EXPECT_EQ(actual6, expected6);

    const uint32_t expected7 = std::stol("1234567890");
    const uint32_t actual7 = 1234567890U;
    EXPECT_EQ(actual7, expected7);

    const uint64_t expected8 = std::stoul("1234567890");
    const uint64_t actual8 = 1234567890UL;
    EXPECT_EQ(actual8, expected8);
}

// c++11 标准函数测试
TEST(HCSTRING_UNITTEST, ToString) { // NOLINT
    const std::string expected1 = std::to_string(2.1f);
    const std::string actual1 = "2.100000";
    EXPECT_STREQ(actual1.c_str(), expected1.c_str());

    const std::string expected2 = std::to_string(2.1111);
    const std::string actual2 = "2.111100";
    EXPECT_STREQ(actual2.c_str(), expected2.c_str());

    const std::string expected3 = std::to_string(12345);
    const std::string actual3 = "12345";
    EXPECT_STREQ(actual3.c_str(), expected3.c_str());

    const std::string expected4 = std::to_string(1234567890);
    const std::string actual4 = "1234567890";
    EXPECT_STREQ(actual4.c_str(), expected4.c_str());

    const std::string expected5 = std::to_string(1234567890);
    const std::string actual5 = "1234567890";
    EXPECT_STREQ(actual5.c_str(), expected5.c_str());

    const std::string expected6 = std::to_string(12345U);
    const std::string actual6 = "12345";
    EXPECT_STREQ(actual6.c_str(), expected6.c_str());

    const std::string expected7 = std::to_string(1234567890U);
    const std::string actual7 = "1234567890";
    EXPECT_STREQ(actual7.c_str(), expected7.c_str());

    const std::string expected8 = std::to_string(1234567890UL);
    const std::string actual8 = "1234567890";
    EXPECT_STREQ(actual8.c_str(), expected8.c_str());
}

TEST(HCSTRING_UNITTEST, Split) { // NOLINT
    std::vector<std::string> v1;
    const std::string s1("a\nb+c");

    hhhstring::split(s1, &v1, "\n+");
    EXPECT_EQ(3U, v1.size());
    EXPECT_STREQ("a", v1[0].c_str());
    EXPECT_STREQ("b", v1[1].c_str());
    EXPECT_STREQ("c", v1[2].c_str());

    std::vector<std::string> v2;
    const std::string s2("a\nb\nc\n");

    hhhstring::split(s2, &v2, "\n");
    EXPECT_EQ(4U, v2.size());
    EXPECT_STREQ("a", v2[0].c_str());
    EXPECT_STREQ("b", v2[1].c_str());
    EXPECT_STREQ("c", v2[2].c_str());
    EXPECT_STREQ("", v2[3].c_str());

    std::vector<std::string> v3;
    const std::string s3("\n");

    hhhstring::split(s3, &v3, "\n");
    EXPECT_EQ(2U, v3.size());
    EXPECT_STREQ("", v3[0].c_str());
    EXPECT_STREQ("", v3[1].c_str());

    std::vector<std::string> v4;
    const std::string s4("abc.com");

    hhhstring::split(s4, &v4, "\n");
    EXPECT_EQ(1U, v4.size());

    std::vector<std::string> v5;
    const std::string s5;

    hhhstring::split(s5, &v5, "\n");
    EXPECT_EQ(0U, v5.size());
}

TEST(HCSTRING_UNITTEST, ToMap1) { // NOLINT
    std::vector<std::string> v{"a", "b", "c"};
    std::map<std::string, std::string> m;

    hhhstring::toMap(v, &m);

    EXPECT_EQ(v.size(), m.size());
    EXPECT_STREQ("", m["a"].c_str());
    EXPECT_STREQ("", m["b"].c_str());
    EXPECT_STREQ("", m["c"].c_str());
}

TEST(HCSTRING_UNITTEST, ToMap2) { // NOLINT
    std::map<std::string, std::string> m1;
    const std::string s1("a\n"
                         "b\n"
                         "c\n");

    hhhstring::toMap(s1, &m1, "\n");
    EXPECT_EQ(4U, m1.size());
    EXPECT_STREQ("", m1["a"].c_str());
    EXPECT_STREQ("", m1["b"].c_str());
    EXPECT_STREQ("", m1["c"].c_str());
    EXPECT_STREQ("", m1[""].c_str());

    std::map<std::string, std::string> m2;
    const std::string s2("\n");

    hhhstring::toMap(s2, &m2, "\n");
    EXPECT_EQ(1U, m2.size());
    EXPECT_STREQ("", m2[""].c_str());

    std::map<std::string, std::string> m3;
    const std::string s3;

    hhhstring::toMap(s3, &m3, "\n");
    EXPECT_EQ(0U, m3.size());
}

TEST(HCSTRING_UNITTEST, ToLong) { // NOLINT
    const long result1 = hhhstring::toLong("123456789");
    EXPECT_EQ(123456789L, result1);

    const long result2 = hhhstring::toLong("-123456789");
    EXPECT_EQ(-123456789L, result2);
}

TEST(HCSTRING_UNITTEST, hcformat) { // NOLINT
    const std::string result = hhhstring::hcformat("Hello %s%s", "world", "!");
    EXPECT_EQ("Hello world!", result);
}

TEST(HCSTRING_UNITTEST, toHexString) { // NOLINT
    std::string result = hhhstring::toHexString("123", "");
    EXPECT_EQ("313233", result);

    result = hhhstring::toHexString("123+=", " ");
    EXPECT_EQ("31 32 33 2B 3D", result);

    result = hhhstring::toHexString("123+=", " ", false);
    EXPECT_EQ("31 32 33 2b 3d", result);
}

TEST(HCSTRING_UNITTEST, fromHexString) { // NOLINT
    const std::string result1 = hhhstring::fromHexString("303132");
    EXPECT_EQ("012", result1);

    const std::string result2 = hhhstring::fromHexString("30 31 32 41 ", 1);
    EXPECT_EQ("012A", result2);

    const std::string result3 = hhhstring::fromHexString("30 20 0A 0D 32 7E", 1, false);
    EXPECT_EQ("0 \n\r2~", result3);

    const std::string result4 = hhhstring::fromHexString("30 20 ZZ 0A 0D 32 7F", 1, true);
    EXPECT_EQ("0....2.", result4);
}



int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
