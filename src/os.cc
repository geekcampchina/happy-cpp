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

#include "happycpp/os.h"
#include <map>

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#include "happycpp/filesys.h"
#include "happycpp/algorithm/hcstring.h"
#include "happycpp/regex.h"
#include <vector>
#include <iostream>

using happycpp::hcalgorithm::hcstring::find;
using happycpp::hcalgorithm::hcstring::split;
using happycpp::hcalgorithm::hcstring::toLower;

namespace happycpp::hcos {

#ifdef PLATFORM_WIN32
    // 是否 64 位操作系统平台
    // 最小支持 Windows XP with SP2/Windows Server 2003 with SP1
    // Windows XP/2003 的支持生命周期已经结束，程序也不需要支持比最小支持版本更老的
    // 操作系统。
#endif

    bool is64BitArch() {
#ifdef PLATFORM_64
        return true;
#endif

#ifdef PLATFORM_WIN32
        bool is_wow64 = false;
        // 32 位程序运行在 WOW64 模式下， is_wow64 等于 true，表示操作系统是 64 位。
        // 32 位程序运行在非 WOW64 模式下， is_wow64 等于 false，表示操作系统是
        // 32 位。
        HAPPY_ASSERT(IsWow64Process(
          GetCurrentProcess(), reinterpret_cast<PBOOL>(&is_wow64)));

        return is_wow64;
#endif
    }

    HAPPYCPP_SHARED_LIB_API std::string getEnumDesc(const std::map<uint32_t, std::string> &desc,
                                                    const uint32_t e) {
        auto it = desc.find(e);

        if (it == desc.end())
            return "";

        return it->second;
    }

    std::string getOsVersionDesc(uint32_t e) {
        static std::map<uint32_t, std::string> desc{
#ifdef PLATFORM_WIN32
                {kWindowsXP, "Windows XP"},
                {kWindowsVista, "Windows Vista"},
                {kWindows7, "Windows 7"},
                {kWindows8, "Windows 8"},
                {kWindows8_1, "Windows 8.1"},
                {kWindowsServer2003, "Windows Server 2003"},
                {kWindowsServer2003R2, "Windows Server 2003 R2"},
                {kWindowsServer2008, "Windows Server 2008"},
                {kWindowsServer2008R2, "Windows Server 2008 R2"},
                {kWindowsServer2012, "Windows Server 2012"},
                {kWindowsServer2012R2, "Windows Server 2012 R2"}
#else
                {kCentOS5, "CentOS 5"},
                {kCentOS6, "CentOS 6"},
                {kCentOS7, "CentOS 7"},
                {kRedHat5, "Red Hat Enterprise Linux 5"},
                {kRedHat6, "Red Hat Enterprise Linux 6"},
                {kRedHat7, "Red Hat Enterprise Linux 7"},
                {kUbuntu12_04, "Ubuntu 12.04"},
                {kUbuntu14_04, "Ubuntu 14.04"},
                {kUbuntu16_04, "Ubuntu 16.04"}
#endif
        };

        return getEnumDesc(desc, e);
    }

    HAPPYCPP_SHARED_LIB_API std::string getOsIdDesc(uint32_t e) {
        static std::map<uint32_t, std::string> desc{
#ifdef PLATFORM_WIN32
                {kWindowsWorkStation, "Windows"},
                {kWindowsServer, "Windows Server"}
#else
                {kCentOS, "CentOS"},
                {kRedHat, "Red Hat Enterprise Linux"},
                {kUbuntu, "Ubuntu"}
#endif
        };

        return getEnumDesc(desc, e);
    }

#ifdef PLATFORM_WIN32
    std::string GetWindowsVariantDesc(const uint32_t e) {
      static std::map<uint32_t, std::string> desc {
        {kDataCenter, "Datacenter Edition"},
        {kEnterprise, "Enterprise Edition"},
        {kProfessional, "Professional Edition"},
        {kStandard, "Standard Edition"},
        {kUltimate, "Ultimate Edition"},
        {kWebServer, "Web Edition"},
        {kSp1, "Service Pack 1"},
        {kSp2, "Service Pack 2"},
        {kSp3, "Service Pack 3"}
      };

      return GetEnumDesc(desc, e);
    }

    std::string GetSpVersionDesc(const uint32_t e) {
      static std::map<uint32_t, std::string> desc {
        {kSp1, "Service Pack 1"},
        {kSp2, "Service Pack 2"},
        {kSp3, "Service Pack 3"}
      };

      return GetEnumDesc(desc, e);
    }

    bool ToVariantType(const uint32_t product, uint32_t *variant) {
      static std::map<uint32_t, WindowsVariantType> desc {
        {PRODUCT_DATACENTER_SERVER, kDataCenter},
        {PRODUCT_DATACENTER_EVALUATION_SERVER, kDataCenter},
        {PRODUCT_ENTERPRISE, kEnterprise},
        {PRODUCT_ENTERPRISE_SERVER, kEnterprise},
        {PRODUCT_PROFESSIONAL, kProfessional},
        {PRODUCT_STANDARD_SERVER, kStandard},
        {PRODUCT_STANDARD_EVALUATION_SERVER , kStandard},
        {PRODUCT_ULTIMATE, kUltimate},
        {PRODUCT_WEB_SERVER, kWebServer}
      };

      std::map<uint32_t, WindowsVariantType>::const_iterator it =
          desc.find(product);

      if (it == desc.end())
        return false;

      *variant = it->second;
      return true;
    }

    bool WindowsMajorVersion5(const OSVERSIONINFOEX &os,
                              const SYSTEM_INFO &sys_info,
                              const uint64_t variant_id, const int testing_level,
                              OsIdentification *osi) {
      OsIdentification _osi;

      if (os.dwMinorVersion == 1) {
        _osi.version = kWindowsXP;

        if (variant_id & VER_SUITE_PERSONAL)
          // Windows XP Home Edition
          return false;
        else
          _osi.variant_id = kProfessional;

        _osi.id = kWindowsWorkStation;
      } else if (os.dwMinorVersion == 2) {
        if (testing_level == 1 || ::GetSystemMetrics(SM_SERVERR2)) {
          _osi.version = kWindowsServer2003R2;
        } else if (variant_id & VER_SUITE_WH_SERVER) {
          // Windows Home Server
          return false;
        } else if (os.wProductType == VER_NT_WORKSTATION &&
              sys_info.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) {
          // Windows XP Professional x64 Edition
          return false;
        } else {
          _osi.version = kWindowsServer2003;
        }

        _osi.id = kWindowsServer;

        if (variant_id & VER_SUITE_DATACENTER)
          _osi.variant_id = kDataCenter;
        else if (variant_id & VER_SUITE_ENTERPRISE)
          _osi.variant_id = kEnterprise;
        else if (variant_id & VER_SUITE_BLADE)
          _osi.variant_id = kWebServer;
        else
          _osi.variant_id = kStandard;
      }

      *osi = _osi;
      return true;
    }

    bool WindowsMajorVersion6(const OSVERSIONINFOEX &os,
                              const SYSTEM_INFO &sys_info,
                              const uint64_t variant_id,
                              OsIdentification *osi) {
      OsIdentification _osi;
      const bool is_workstation = (os.wProductType == VER_NT_WORKSTATION);
      _osi.id = is_workstation ? kWindowsWorkStation : kWindowsServer;

      switch (os.dwMinorVersion) {
        case 0:
          _osi.version = is_workstation ? kWindowsVista : kWindowsServer2008;
          break;
        case 1:
          _osi.version = is_workstation ? kWindows7 : kWindowsServer2008R2;
          break;
        case 2:
          _osi.version = is_workstation ? kWindows8 : kWindowsServer2012;
          break;
        case 3:
          _osi.version = is_workstation ? kWindows8_1 : kWindowsServer2012R2;
          break;
        default:
          return false;
      }

      if (ToVariantType(variant_id, &_osi.variant_id)) {
        *osi = _osi;
        return true;
      }

      return false;
    }

    void HappyGetSystemInfo(SYSTEM_INFO *sys_info) {
      typedef void (WINAPI *FunGetNativeSystemInfo)(LPSYSTEM_INFO);

      FunGetNativeSystemInfo fun_gnsi =
          reinterpret_cast<FunGetNativeSystemInfo>(
              GetProcAddress(
                GetModuleHandle(TEXT("kernel32.dll")), "GetNativeSystemInfo"));

      if (fun_gnsi == nullptr)
        GetSystemInfo(sys_info);
      else
        // 运行于 WOW64 模式时，使用 GetNativeSystemInfo() 才能接收准确的系统信息
        fun_gnsi(sys_info);
    }

    uint64_t GetVariantId(const OSVERSIONINFOEX &os) {
      typedef BOOL (WINAPI *FunGetProductInfo)(DWORD, DWORD, DWORD, DWORD, PDWORD);

      FunGetProductInfo fun_gpi =
          reinterpret_cast<FunGetProductInfo>(
              GetProcAddress(
                GetModuleHandle(TEXT("kernel32.dll")), "GetProductInfo"));

      uint64_t variant_id = 0;

      if (fun_gpi == nullptr)
        variant_id = os.wSuiteMask;
      else
        fun_gpi(os.dwMajorVersion, os.dwMinorVersion, 0, 0,
                reinterpret_cast<PDWORD>(&variant_id));

      return variant_id;
    }

    void MakePrettyName(OsIdentification *osi) {
      const std::string sp_version = GetSpVersionDesc(osi->sp_version);

      osi->pretty_name =
        GetOsVersionDesc(osi->version) + " " +
        GetWindowsVariantDesc(osi->variant_id) +
        (sp_version.empty() ? "" : " ") + sp_version;
    }

    bool _GetOsInfo(void *sys_info, void *os, const uint64_t variant_id,
                    const int testing_level, OsIdentification *osi) {
      SYSTEM_INFO *_sys_info = reinterpret_cast<SYSTEM_INFO *>(sys_info);
      OSVERSIONINFOEX *_os = reinterpret_cast<OSVERSIONINFOEX *>(os);

      switch (_os->dwMajorVersion) {
        case 5:
          if (WindowsMajorVersion5(*_os, *_sys_info, variant_id, testing_level, osi))
            break;

          return false;
        case 6:
          if (WindowsMajorVersion6(*_os, *_sys_info, variant_id, osi))
            break;

          return false;
        default:
          // 不支持的 Windows 版本
          HAPPY_ASSERT(false);
      }

      osi->major_version = _os->dwMajorVersion;
      osi->minor_version = _os->dwMinorVersion;
      osi->build_id = _os->dwBuildNumber;
      osi->sp_version = _os->wServicePackMajor;

      if (_sys_info->wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64)
        osi->arch_id = 64;
      else
        osi->arch_id = 32;

      MakePrettyName(osi);
      return true;
    }
#else

    HAPPYCPP_SHARED_LIB_API bool getRedHatRelease(const char *info, OsIdentification *osi) {
        OsIdentification _osi;
        std::string _info;

        if (info) {
            _info = info;
        } else {
            if (bfs::exists("/etc/redhat-release"))
                _info = hcfilesys::readFile("/etc/redhat-release");
            else
                return false;
        }

        static HappyRegex re = happyRegexCompile("^(.*) "
                                                 "release "
                                                 "(\\d+)\\.(\\d+)(?:\\.(\\d+))? "
                                                 "\\((.+)\\)");

        HappySmatch what;

        if (!happyRegexMatch(_info, what, re))
            return false;

        const std::string name(what[1]);

        if (find(name, "CentOS"))
            _osi.id = kCentOS;
        else if (find(name, "Red Hat Enterprise Linux"))
            _osi.id = kRedHat;

        _osi.arch_id = is64BitArch() ? 64 : 32;
        _osi.major_version = std::stoi(what[2]);
        _osi.minor_version = std::stoi(what[3]);
        _osi.build_id = toLower(what[4]);
        _osi.code_id = toLower(what[5]);
        _osi.pretty_name = info;

        const bool is_centos = (_osi.id == kCentOS);

        switch (_osi.major_version) {
            case 5:
                _osi.version = is_centos ? kCentOS5 : kRedHat5;
                break;
            case 6:
                _osi.version = is_centos ? kCentOS6 : kRedHat6;
                break;
            case 7:
                _osi.version = is_centos ? kCentOS7 : kRedHat7;
                break;
            default:
                break;
        }

        *osi = _osi;
        return true;
    }

    void line2map(const std::string &s, std::map<std::string, std::string> *m) {
        // x=
        if (s.size() < 2)
            return;

        std::vector<std::string> tmp_list;

        if (split(s, &tmp_list, "=") && tmp_list.size() == 2) {
            if (tmp_list[1].size() > 2) {
                auto begin_it = tmp_list[1].begin();
                if (*begin_it == '"') tmp_list[1].erase(begin_it);

                auto last_it = tmp_list[1].end() - 1;
                if (*last_it == '"') tmp_list[1].erase(last_it);
            }

            m->insert(std::pair<std::string, std::string>(tmp_list[0], tmp_list[1]));
        }
    }

    HAPPYCPP_SHARED_LIB_API bool getLsbRelease(const char *info, OsIdentification *osi) {
        OsIdentification _osi;
        std::string _info;

        if (info) {
            _info = info;
        } else {
            if (bfs::exists("/etc/lsb-release"))
                _info = hcfilesys::readFile("/etc/lsb-release");
            else
                return false;
        }

        std::vector<std::string> line_list;
        std::map<std::string, std::string> directive_list;

        if (!split(_info, &line_list, EOL))
            return false;

        for (const auto &line : line_list)
            line2map(line, &directive_list);

        if (directive_list.empty())
            return false;

        for (const auto &it : directive_list) {
            if (it.first == "LSB_VERSION") {
                return false;
            } else if (it.first == "DISTRIB_ID") {
                if (toLower(it.second) == "ubuntu")
                    _osi.id = kUbuntu;
                else
                    return false;
            } else if (it.first == "DISTRIB_RELEASE") {
                if (_osi.id == kUbuntu) {
                    if (it.second == "12.04")
                        _osi.version = kUbuntu12_04;
                    else if (it.second == "14.04")
                        _osi.version = kUbuntu14_04;
                    else if (it.second == "16.04")
                        _osi.version = kUbuntu16_04;
                    else
                        return false;
                } else {
                    return false;
                }

                _osi.major_version = 0;
                _osi.minor_version = 0;
                sscanf(it.second.c_str(), // NOLINT
                       "%u.%u",
                       &_osi.major_version, &_osi.minor_version);
            } else if (it.first == "DISTRIB_CODENAME") {
                _osi.code_id = toLower(it.second);
            } else if (it.first == "DISTRIB_DESCRIPTION") {
                _osi.pretty_name = it.second;
            } else {
                // 所有键值遍历完毕，未找到匹配的，直接返回 false
                return false;
            }
        }

        _osi.arch_id = is64BitArch() ? 64 : 32;
        *osi = _osi;
        return true;
    }

    HAPPYCPP_SHARED_LIB_API bool getOsRelease(const char *info, OsIdentification *osi) {
        OsIdentification _osi;
        std::string _info;

        if (info) {
            _info = info;
        } else {
            if (bfs::exists("/etc/os-release"))
                _info = hcfilesys::readFile("/etc/os-release");
            else
                return false;
        }

        std::vector<std::string> line_list;
        std::map<std::string, std::string> directive_list;
        std::string version_id;

        if (!split(_info, &line_list, EOL))
            return false;

        for (const auto &line : line_list)
            line2map(line, &directive_list);

        if (directive_list.empty())
            return false;

        int match_key = 0;

        for (const auto &it : directive_list) {
            if (it.first == "ID") {
                ++match_key;

                if (toLower(it.second) == "centos")
                    _osi.id = kCentOS;
                else if (toLower(it.second) == "ubuntu")
                    _osi.id = kUbuntu;
                else
                    return false;
            } else if (it.first == "VERSION_ID") {
                ++match_key;

                if (_osi.id == kUbuntu) {
                    if (it.second == "14.04")
                        _osi.version = kUbuntu14_04;
                    else if (it.second == "16.04")
                        _osi.version = kUbuntu16_04;
                    else
                        return false;
                } else if (_osi.id == kCentOS) {
                    if (it.second == "7")
                        _osi.version = kCentOS7;
                    else
                        return false;
                } else {
                    return false;
                }

                _osi.major_version = 0;
                _osi.minor_version = 0;
                sscanf(it.second.c_str(), // NOLINT
                       "%u.%u",
                       &_osi.major_version, &_osi.minor_version);
            } else if (it.first == "PRETTY_NAME") {
                ++match_key;

                _osi.pretty_name = it.second;
            } else {
                continue;
            }
        }

        if (match_key != 3)
            return false;

        _osi.arch_id = is64BitArch() ? 64 : 32;
        *osi = _osi;
        return true;
    }

    HAPPYCPP_SHARED_LIB_API bool _getOsInfo(const char *info, OsIdentification *osi) {
        if (getOsRelease(info, osi))
            return true; // NOLINT
        else if (getLsbRelease(info, osi))
            return true;
        else if (getRedHatRelease(info, osi))
            return true;
        else
            return false;
    }

#endif

    HAPPYCPP_SHARED_LIB_API bool getOsInfo(OsIdentification *osi) {
#ifdef PLATFORM_WIN32
        SYSTEM_INFO sys_info;
        HappyGetSystemInfo(&sys_info);

        OSVERSIONINFOEX os;
        os.dwOSVersionInfoSize = sizeof(os);
        HAPPY_ASSERT(::GetVersionEx(reinterpret_cast<OSVERSIONINFO *>(&os)));

        const uint64_t variant_id = GetVariantId(os);

        return _GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0, osi);
#else
        return _getOsInfo(nullptr, osi);
#endif
    }

} /* namespace happycpp */
