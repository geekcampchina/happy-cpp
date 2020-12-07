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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_IP_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_IP_H_

#include "happycpp/common.h"
#include <string>

namespace happycpp {

    namespace hcalgorithm {

        namespace hcip {

            HAPPYCPP_SHARED_LIB_API bool IsIpAddr(const std::string &ip);

/*
是否是保留的IP地址，包括私有地址、广播地址等等。
非IP地址或者IP转换失败，抛出异常
IP地址分为 保留IP地址 和 公网IP地址(非保留IP地址)

保留范围:
CIDR地址块       点分十进制                    用十进制写出的32位整数(网络字节)
0.0.0.0/8       0.0.0.0-0.255.255.255       0          - 16777215
10.0.0.0/8      10.0.0.0-10.255.255.255     167772160  - 184549375
127.0.0.0/8     127.0.0.0-127.255.255.255   2130706432 - 2147483647
169.254.0.0/16  169.254.0.0-169.254.255.255 2851995648 - 2852061183
172.16.0.0/12   172.16.0.0-172.31.255.255   2886729728 - 2887778303
192.0.0.0/24    192.0.0.0-192.0.0.255       3221225472 - 3221225727
192.0.2.0/24    192.0.2.0-192.0.2.255       3221225984 - 3221226239
192.88.99.0/24  192.88.99.0-192.88.99.255   3227017984 - 3227018239
192.168.0.0/16  192.168.0.0-192.168.255.255 3232235520 - 3232301055
198.18.0.0/15   198.18.0.0-198.19.255.255   3323068416 - 3323199487
198.51.100.0/24 198.51.100.0-198.51.100.255 3325256704 - 3325256959
203.0.113.0/24  203.0.113.0-203.0.113.255   3405803776 - 3405804031
224.0.0.0/4     224.0.0.0-239.255.255.255   3758096384 - 4026531839
240.0.0.0/4     240.0.0.0-255.255.255.255   4026531840 - 4294967295
255.255.255.255/32      255.255.255.255     4294967295

参考：
http://zh.wikipedia.org/wiki/IPv4
http://en.wikipedia.org/wiki/Reserved_IP_addresses#Reserved_IPv4_addresses
*/
            bool IsReserveIpAddr(const std::string &ip);

/* 验证ip/cidr，比如 125.65.110.0/24，cidr有效范围是8到32 */
            bool IsIpCidr(const std::string &s);

        } /* namespace hcip */

    } /* namespace hcalgorithm */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_IP_H_
