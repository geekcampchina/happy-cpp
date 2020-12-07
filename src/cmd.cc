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

#include "happycpp/cmd.h"
#include "happycpp/common.h"
#include "happycpp/log.h"
#include "happycpp/algorithm/hcstring.h"
#include <cstdlib>
#include <cstdio>

#ifdef PLATFORM_WIN32
#ifndef INCLUDE_WINDOWS_H_FILE
#define INCLUDE_WINDOWS_H_FILE
#include <Windows.h>
#endif
#endif

#ifdef PLATFORM_WIN32
#define popen _popen
#define pclose _pclose
#define ToNull " >nul 2>&1"
#else
#define ToNull " >/dev/null 2>&1"
#endif

using std::to_string;
using happycpp::hcalgorithm::hcstring::Trim;

namespace happycpp {

    namespace hccmd {

        HAPPYCPP_SHARED_LIB_API bool GetExitStatusOfCmd(const std::string &cmd) {
            happycpp::log::HappyLogPtr hlog = happycpp::log::HappyLog::getInstance();
            hlog->trace("cmd=" + cmd + EOL);

            // 重定向输出到null
            const std::string redirect_cmd(cmd + ToNull);
            const bool ret = (system(redirect_cmd.c_str()) == 0);

            if (ret)
                hlog->trace("ret=" EOL "successful");
            else
                hlog->trace("ret=" EOL "unsuccessful");

            return ret;
        }

        HAPPYCPP_SHARED_LIB_API std::string GetOutputOfCmd(const std::string &cmd) {
            happycpp::log::HappyLogPtr hlog = happycpp::log::HappyLog::getInstance();
            hlog->trace("cmd=" + cmd + EOL);

            int32_t size = 2048;
            char buffer[2048];
            std::string ret;

            FILE *stream = popen(cmd.c_str(), "r");

            if (stream != nullptr) {
                // 每次从 stream 中读取指定大小的内容
                while (fgets(buffer, size, stream))
                    ret += buffer;

                pclose(stream);
            }

            ret = Trim(ret, " \r\n");
            hlog->trace("ret=" + cmd + EOL);

            return ret;
        }

#ifdef PLATFORM_WIN32

        HAPPYCPP_SHARED_LIB_API void ExecuteCmdWithSubProc(
            const std::string &cmd, const uint32_t &delay_secs) {
          happycpp::log::HappyLogPtr hlog = happycpp::log::HappyLog::getInstance();
          hlog->trace("cmd=" + cmd + EOL);
          hlog->trace("delay_secs=" + cmd + std::atoi(delay_secs));

          STARTUPINFO startup_info;
          PROCESS_INFORMATION proc_info;
          ZeroMemory(&startup_info, sizeof(startup_info));
          startup_info.cb = sizeof(startup_info);
          ZeroMemory(&proc_info, sizeof(proc_info));

          std::string _cmd("cmd /c ");

          if (delay_secs > 0)
            _cmd += "ping 127.0.0.1 -n " + to_string(delay_secs) + " > nul &" + cmd;
          else
            _cmd += cmd;

          const bool ret_val = CreateProcess(
            nullptr,     // No module name (use command line)
            const_cast<LPSTR>(cmd.c_str()),  // Command line
            nullptr,     // Process handle not inheritable
            nullptr,     // Thread handle not inheritable
            FALSE,    // Set handle inheritance to FALSE
            0,        // No creation flags
            nullptr,     // Use parent's environment block
            nullptr,     // Use parent's starting directory
            &startup_info,      // Pointer to STARTUPINFO structure
            &proc_info);      // Pointer to PROCESS_INFORMATION structure

          if (ret_val) {
            // 关闭返回的子进程的进程句柄和主线程句柄
            CloseHandle(proc_info.hProcess);
            CloseHandle(proc_info.hThread);

            // 不等待子进程返回
            WaitForSingleObject(proc_info.hProcess, 0);
          } else {
            // 非逻辑错误，属于异常情况
            ThrowHappyException("Can not execute the command.");
          }
        }

#endif

    } /* namespace hccmd */

} /* namespace happycpp */
