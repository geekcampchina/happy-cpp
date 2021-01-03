// -*- C++ -*-
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

/** @file */

#ifndef INCLUDE_HAPPYCPP_PROC_H_
#define INCLUDE_HAPPYCPP_PROC_H_

#include "happycpp/common.h"
#include <string>
#include <vector>

namespace happycpp::hcproc {

    // 返回当前工作目录绝对路径
    HAPPYCPP_SHARED_LIB_API bool currentWorkDir(std::string *dir);

    // 返回当前进程文件绝对路径
    HAPPYCPP_SHARED_LIB_API bool currentExePath(std::string *path);

    // 返回当前进程文件所在目录(执行目录)
    HAPPYCPP_SHARED_LIB_API bool currentExeDir(std::string *dir);

#ifdef PLATFORM_WIN32
    // 创建进程锁，防止运行多个进程实例
    HAPPYCPP_SHARED_LIB_API bool LockProc(const std::string &proc);

    // 解锁进程锁
    HAPPYCPP_SHARED_LIB_API bool UnLockProc(const std::string &proc);
#else
    // 创建进程锁，防止运行多个进程实例
    HAPPYCPP_SHARED_LIB_API bool lockProc(const std::string &proc, pid_t pid,
                                          const std::string &prefix = "/var/run");

    // 解锁进程锁
    HAPPYCPP_SHARED_LIB_API bool unLockProc(
            const std::string &proc, const std::string &prefix = "/var/run");

#endif

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_PROC_H_
