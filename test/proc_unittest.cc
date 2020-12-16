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
#include "happycpp/filesys.h"
#include "happycpp/proc.h"
#include "happycpp/algorithm/hcstring.h"

namespace hhproc = happycpp::hcproc;
namespace hhhstring = happycpp::hcalgorithm::hcstring;

std::string format_path(const std::string &path) {
    return hhhstring::replace(path, "/", OsSeparator);
}

TEST(HCPROC_UNITTEST, CurrentWorkDir) { // NOLINT
    std::string expected;
    EXPECT_TRUE(hhproc::currentWorkDir(&expected));

#ifdef CURRENT_WORK_DIR
    const std::string actual(format_path(CURRENT_WORK_DIR));
    EXPECT_STREQ(actual.c_str(), expected.c_str());
#endif
}

TEST(HCPROC_UNITTEST, CurrentExePath) { // NOLINT
    std::string expected;
    EXPECT_TRUE(hhproc::currentExePath(&expected));

#ifdef CURRENT_EXE_PATH
    const std::string actual(format_path(CURRENT_EXE_PATH));
    EXPECT_STREQ(actual.c_str(), expected.c_str());
#endif
}

TEST(HCPROC_UNITTEST, CurrentExeDir) { // NOLINT
    std::string expected;
    EXPECT_TRUE(hhproc::currentExeDir(&expected));

#ifdef CURRENT_EXE_DIR
    const std::string actual(format_path(CURRENT_EXE_DIR));
    EXPECT_STREQ(actual.c_str(), expected.c_str());
#endif
}

TEST(HCPROC_UNITTEST, LockProcAndUnLockProc) { // NOLINT
#ifdef PLATFORM_WIN32
    EXPECT_FALSE(hhproc::UnLockProc("test.lock"));
    EXPECT_TRUE(hhproc::LockProc("test.lock"));

    EXPECT_FALSE(hhproc::LockProc("test.lock"));
    EXPECT_TRUE(hhproc::UnLockProc("test.lock"));
#else
    EXPECT_FALSE(hhproc::unLockProc("test.lock", "/tmp"));
    EXPECT_TRUE(hhproc::lockProc("test.lock", getppid(), "/tmp"));

    EXPECT_FALSE(hhproc::lockProc("test.lock", getppid(), "/tmp"));
    EXPECT_TRUE(hhproc::unLockProc("test.lock", "/tmp"));
#endif
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
