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

#include "happycpp/hcerrno.h"
#include <string>

#ifdef PLATFORM_WIN32
#ifndef INCLUDE_WINDOWS_H_FILE
#define INCLUDE_WINDOWS_H_FILE
#include <Windows.h>
#endif
#else

#include <cerrno>

#endif

#include <cstring>

using std::string;
using std::to_string;

namespace happycpp {

    namespace hcerrno {

        HAPPYCPP_SHARED_LIB_API std::string ErrorToStr() {
#ifdef PLATFORM_WIN32
            std::string msg("");
            LPVOID lp_msg_buf = NULL;
            DWORD msg_id = ::GetLastError();

            // Windows API 没有错误
            if (msg_id == 0) {
              // C 运行时没有错误
              if (errno == 0)
                return "";

              // C 运行时有错误
              msg = _strerror(NULL);
              const size_t size = msg.size();

              if (size) msg.erase(size - 1);

              return msg + "(errno: " + std::to_string(errno) + ")";
            }

            // Windows API 有错误
            DWORD retval = ::FormatMessageA(
                FORMAT_MESSAGE_ALLOCATE_BUFFER
                  | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                NULL,
                msg_id,
                MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
                (LPSTR) &lp_msg_buf,
                0,
                NULL);

            msg = (retval == 0 ? "Unknown error" : static_cast<LPCSTR>(lp_msg_buf));
            LocalFree(lp_msg_buf);

            while (msg.size()
                   && (msg[msg.size() - 1] == '\n'
                   || msg[msg.size() - 1] == '\r'))
            msg.erase(msg.size() - 1);

            if (msg.size() && msg[msg.size() - 1] == '.')
            msg.erase(msg.size()-1);

            return msg + "(errno: " + std::to_string(msg_id) + ")";
#else
            if (errno == 0)
                return "";

            return std::string(strerror(errno))
                   + "(errno: " + std::to_string(errno) + ")";
#endif
        }

    } /* namespace hcerrno */

} /* namespace happycpp */
