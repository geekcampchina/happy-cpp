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

#include "happycpp/algorithm/ip.h"

#ifdef PLATFORM_WIN32
#if (_WIN32_WINNT >= 0x0600)  // 0x0600 = Vista
#include <Ws2tcpip.h>  // for InetPton
#include <Inaddr.h>  // for IN_ADDR
#else
#include <winsock2.h>  // for windows server 2003/xp
#include <ws2tcpip.h>
#endif
#else

#include <arpa/inet.h>  // for linux

#endif

namespace happycpp::hcalgorithm::hcip {
#ifdef PLATFORM_WIN32
# if _WIN32_WINNT >= 0x0600  // 0x0600 = Vista

    HAPPYCPP_SHARED_LIB_API bool IsIpAddr(const std::string &s) {
      IN_ADDR sa;
      return InetPton(AF_INET, s.c_str(), &sa) == 1;
    }

# else

    // https://github.com/wireshark/wireshark/blob/master/wsutil/inet_pton.c
    int _inet_pton(const char *src, u_char *dst) {
      static const int NS_INADDRSZ = 4;
      static const char digits[] = "0123456789";
      int saw_digit = 0;
      int octets = 0;
      int ch = 0;
      u_char tmp[NS_INADDRSZ];
      u_char *tp = NULL;

      *(tp = tmp) = 0;

      while ((ch = *src++) != '\0') {
        const char *pch;

        if ((pch = strchr(digits, ch)) != NULL) {
          size_t c = *tp * 10 + (pch - digits);

          if (c > 255) return 0;

          *tp = (u_char) c;

          if (!saw_digit) {
            if (++octets > 4) return 0;

            saw_digit = 1;
          }
        } else if (ch == '.' && saw_digit) {
          if (octets == 4) return 0;

          *++tp = 0;
          saw_digit = 0;
        } else {
          return 0;
        }
      }

      if (octets < 4)
        return 0;

      memcpy(dst, tmp, NS_INADDRSZ);
      return 1;
    }

    HAPPYCPP_SHARED_LIB_API bool IsIpAddr(const std::string &s) {
      u_char dst[50];
      return _inet_pton(s.c_str(), dst) == 1;
    }

# endif
#else

    HAPPYCPP_SHARED_LIB_API bool isIpAddr(const std::string &s) {
        struct sockaddr_in sa{};
        return inet_pton(AF_INET, s.c_str(), &(sa.sin_addr)) == 1;
    }

#endif

    HAPPYCPP_SHARED_LIB_API bool isReserveIpAddr(const std::string &ip) {
        if (!isIpAddr(ip))
            return false;

        // 用十进制写出的32位整数
        struct in_addr decimal_addr{};

        const int ret_val = inet_pton(AF_INET, ip.c_str(), &decimal_addr);

        // Can not convert Internet host address from numbers-and-dots notation.
        HAPPY_ASSERT(ret_val != 0);

        // 转换为网络字节
        const uint32_t host_addr = htonl(decimal_addr.s_addr);

        return ((host_addr >= 0 && host_addr <= 16777215)
                || (host_addr >= 167772160 && host_addr <= 184549375)
                || (host_addr >= 2130706432 && host_addr <= 2147483647)
                || (host_addr >= 2851995648 && host_addr <= 2852061183)
                || (host_addr >= 2886729728 && host_addr <= 2887778303)
                || (host_addr >= 3221225472 && host_addr <= 3221225727)
                || (host_addr >= 3221225984 && host_addr <= 3221226239)
                || (host_addr >= 3227017984 && host_addr <= 3227018239)
                || (host_addr >= 3232235520 && host_addr <= 3232301055)
                || (host_addr >= 3323068416 && host_addr <= 3323199487)
                || (host_addr >= 3325256704 && host_addr <= 3325256959)
                || (host_addr >= 3405803776 && host_addr <= 3405804031)
                || (host_addr >= 3758096384 && host_addr <= 4026531839)
                || (host_addr >= 4026531840 && host_addr <= 4294967295));
    }

    HAPPYCPP_SHARED_LIB_API bool isIpCidr(const std::string &s) {
        const int kMinCidr = 8;
        const int kMaxCidr = 32;

        if (s.empty()) return false;

        const size_t sep_size_(1);
        const size_t pos_ = s.find_first_of('/');

        if (pos_ == std::string::npos)
            return false;

        const std::string ip(s.substr(0, pos_));

        if (!isIpAddr(ip)) return false;

        const std::string cidr_str(s.substr(pos_ + sep_size_));

        if (cidr_str.empty()) return false;

        const uint16_t cidr = stoi(cidr_str);

        return (cidr >= kMinCidr && cidr <= kMaxCidr);
    }

} /* namespace happycpp */

