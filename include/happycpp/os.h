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

#ifndef INCLUDE_HAPPYCPP_OS_H_
#define INCLUDE_HAPPYCPP_OS_H_

#include "happycpp/common.h"
#include <string>
#include <map>

namespace happycpp {

    namespace hcos {

// 操作系统发行版类型列表
        enum OsIdType {
#ifdef PLATFORM_WIN32
            kWindowsWorkStation,
            kWindowsServer
#else
            kCentOS,
            kRedHat,
            kUbuntu
#endif
        };

// Linux 发行版
        enum OsVersion {
#ifdef PLATFORM_WIN32
            kWindowsXP,
            kWindowsVista,
            kWindows7,
            kWindows8,
            kWindows8_1,
            kWindowsServer2003,
            kWindowsServer2003R2,
            kWindowsServer2008,
            kWindowsServer2008R2,
            kWindowsServer2012,
            kWindowsServer2012R2
#else
            kCentOS5,
            kCentOS6,
            kCentOS7,
            kRedHat5,
            kRedHat6,
            kRedHat7,
            kUbuntu12_04,
            kUbuntu14_04,
            kUbuntu16_04
#endif
        };

#ifdef PLATFORM_WIN32
        // Windows 发行版变种
        enum WindowsVariantType {
          kDataCenter,
          kEnterprise,
          kProfessional,
          kStandard,
          kUltimate,
          kWebServer
        };

        enum SpVersion{
          kSp1 = 1,
          kSp2,
          kSp3,
        };
#endif

// 操作系统身份证
        struct OsIdentification {
            uint32_t id;
            uint32_t version;
            uint32_t major_version;
            uint32_t minor_version;
            uint32_t arch_id;
#ifdef PLATFORM_WIN32
            uint32_t sp_version;
            uint32_t variant_id;
            uint32_t build_id;
#else
            std::string build_id;
            std::string code_id;  // 代号
#endif
            std::string pretty_name;
        };

#ifdef PLATFORM_WIN32
        // 仅内部和单元测试使用
        bool _GetOsInfo(void *sys_info, void *os, const uint64_t variant_id,
                        const int testing_level, OsIdentification *osi);

        std::string GetSpVersionDesc(const uint32_t e);
        std::string GetWindowsVariantDesc(const uint32_t e);
#else

// 仅内部和单元测试使用
// info 为 nullptr 表示从文件读取
        bool _GetOsInfo(const char *info, OsIdentification *osi);

#endif

        std::string GetOsVersionDesc(uint32_t e);

        std::string GetOsIdDesc(uint32_t e);

        bool GetOsInfo(OsIdentification *osi);

    } /* namespace hcos */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_OS_H_
