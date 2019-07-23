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
#include <happycpp/filesys.h>
#include <happycpp/exception.h>
#include <cstdlib>

namespace hhfilesys = happycpp::hcfilesys;

TEST(HCFILESYS_UNITTEST, HappyCreateFile) {
  ASSERT_TRUE(hhfilesys::HappyCreateFile("testFile"));
  ASSERT_TRUE(bfs::remove("testFile"));
}

TEST(HCFILESYS_UNITTEST, ReadFile1_and_WriteFile1) {
  ASSERT_TRUE(hhfilesys::WriteFile("testFile", "1234567890"));

  const std::string s = hhfilesys::ReadFile("testFile");
  EXPECT_STREQ("1234567890", s.c_str());

  bfs::remove("testFile");
}

TEST(HCFILESYS_UNITTEST, ReadFile2_and_WriteFile2) {
  std::vector<std::string> v1 { "1234567890", "abcdefg", "ABCDEFG" };
  std::vector<std::string> v2;

  ASSERT_TRUE(hhfilesys::WriteFile("testFile", v1));
  ASSERT_TRUE(hhfilesys::ReadFile("testFile", &v2));
  EXPECT_EQ(3U, v2.size());
  EXPECT_STREQ("1234567890", v2[0].c_str());
  EXPECT_STREQ("abcdefg", v2[1].c_str());
  EXPECT_STREQ("ABCDEFG", v2[2].c_str());

  bfs::remove("testFile");
}

TEST(HCFILESYS_UNITTEST, GetFilesInDir) {
  bfs::create_directories("test_dir" OsSeparator "sub");
  hhfilesys::HappyCreateFile("test_dir" OsSeparator "f1.txt");
  hhfilesys::HappyCreateFile("test_dir" OsSeparator "f2.txt");

  std::vector<FileStat> v;

  EXPECT_NO_THROW(hhfilesys::GetFilesInDir("test_dir", kAll, &v));
  EXPECT_EQ(3U, v.size());

  EXPECT_NO_THROW(hhfilesys::GetFilesInDir("test_dir", kFile, &v));
  EXPECT_EQ(2U, v.size());

  const bool f1 = (v[0].name == "f1.txt" || v[1].name == "f1.txt");
  const bool f2 = (v[0].name == "f2.txt" || v[1].name == "f2.txt");
  EXPECT_TRUE(f1 && f2);

  EXPECT_NO_THROW(hhfilesys::GetFilesInDir("test_dir", kDir, &v));
  EXPECT_EQ(1U, v.size());
  EXPECT_STREQ("sub", v[0].name.c_str());

  bfs::remove_all("test_dir");
}
