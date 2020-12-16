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
#include "happycpp/cmd.h"
#ifdef PLATFORM_WIN32
#include "happycpp/filesys.h"
#include "happycpp/algorithm/hctime.h"
#endif

namespace hhcmd = happycpp::hccmd;

#ifdef PLATFORM_WIN32
namespace hhtime = happycpp::hcalgorithm::hctime;
#endif

TEST(HCCMD_UNITTEST, GetExitStatusOfCmd) { // NOLINT
    std::string cmd;
    bool ret;

    if (bfs::exists("cmdtest")) {
        cmd = "rmdir cmdtest";
        ret = hhcmd::getExitStatusOfCmd(cmd);
        EXPECT_TRUE(ret);
    }

    cmd = "mkdir cmdtest";
    ret = hhcmd::getExitStatusOfCmd(cmd);
    EXPECT_TRUE(ret);

    cmd = "rmdir cmdtest";
    ret = hhcmd::getExitStatusOfCmd(cmd);
    EXPECT_TRUE(ret);
}

TEST(HCCMD_UNITTEST, GetOutputOfCmd) { // NOLINT
    std::string cmd("echo testing");
    std::string ret(hhcmd::getOutputOfCmd(cmd));
    const std::string expected("testing");
    EXPECT_STREQ(expected.c_str(), ret.c_str());
}

#ifdef PLATFORM_WIN32
TEST(HCCMD_UNITTEST, ExecuteCmdWithSubProc) {
  const std::string dir("C:\\Windows\\Temp\\test_dir");
  time_t start, end;
  std::string cmd("mkdir " + dir);

  bfs::remove_all(dir);
  bfs::exists(dir);

  time(&start);
  hhcmd::ExecuteCmdWithSubProc(cmd, 1);
  time(&end);

  const double diff = difftime(end, start);

  // diff < 100 ms
  EXPECT_LT(diff, 0.1);

  // 等待 mkdir 命令执行完成
  hhtime::HappyWait(3);
  EXPECT_TRUE(bfs::exists(dir));
  bfs::remove_all(dir);
}
#endif

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
