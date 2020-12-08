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

#ifndef INCLUDE_HAPPYCPP_CMD_H_
#define INCLUDE_HAPPYCPP_CMD_H_

#include "happycpp/common.h"
#include <string>

namespace happycpp {

    namespace hccmd {

        // 执行系统命令，屏蔽标准输出，根据命令退出代码返回布尔值
        HAPPYCPP_SHARED_LIB_API bool getExitStatusOfCmd(const std::string &cmd);

        // 执行系统命令，返回命令执行结果字符串
        HAPPYCPP_SHARED_LIB_API std::string getOutputOfCmd(const std::string &cmd);

#ifdef PLATFORM_WIN32
        // 使用非阻塞的子进程执行命令
        HAPPYCPP_SHARED_LIB_API void ExecuteCmdWithSubProc(
            const std::string &cmd, const uint32_t &delay_secs = 0);
#else
// Linux 直接使用 sleep + & 即可达到同样的效果。比如，sleep 2 && ls /tmp &
#endif

    } /* namespace hccmd */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_CMD_H_
