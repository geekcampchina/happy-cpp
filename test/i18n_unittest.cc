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
#include "happycpp/i18n.h"

TEST(HAPPYCPP_UNITTEST, WcsToMbs) {
    const std::string s("Hello world!");
    const std::wstring ws(L"Hello world!");

    ASSERT_EQ(s, happycpp::wcsToMbs(ws));
}

TEST(HAPPYCPP_UNITTEST, MbsToWcs) {
    const std::string s("Hello world!");
    const std::wstring ws(L"Hello world!");

    ASSERT_EQ(ws, happycpp::mbsToWcs(s));
}

#ifdef PLATFORM_WIN32
std::string ToRawString(std::string const& s) {
  std::ostringstream out;
  out << std::hex;

  for (auto it : s) {
    const unsigned char uc = static_cast<unsigned char>(it);
    out << static_cast<int>(uc);
  }

  return out.str();
}

TEST(HAPPYCPP_UNITTEST, AnsiToUtf8) {
  const std::string s("你好，世界！");
  const std::string utf8_s(happycpp::AnsiToUtf8(s));

  ASSERT_EQ("e4bda0e5a5bdefbc8ce4b896e7958cefbc81", ToRawString(utf8_s));
}

TEST(HAPPYCPP_UNITTEST, Utf8ToAnsi) {
  const std::string s("你好，世界！");
  const std::string utf8_s(happycpp::AnsiToUtf8(s));

  ASSERT_EQ(s, happycpp::Utf8ToAnsi(utf8_s));
}
#endif