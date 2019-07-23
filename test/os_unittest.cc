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
#include <happycpp/os.h>
#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

namespace hhos = happycpp::hcos;

#ifdef PLATFORM_WIN32
TEST(HCOS_UNITTEST, GetOsInfo_XP) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 5;
  os.dwMinorVersion = 1;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 3;
  os.wServicePackMinor = 0;
  os.wSuiteMask = VER_SUITE_PERSONAL;
  os.wProductType = VER_NT_WORKSTATION;

  uint64_t variant_id = VER_SUITE_PERSONAL;

  ASSERT_FALSE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                          reinterpret_cast<void *>(&os), variant_id, 0,
                          &osi));

  os.dwMinorVersion = 2;
  ASSERT_FALSE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                          reinterpret_cast<void *>(&os), variant_id, 0,
                          &osi));

  os.dwMinorVersion = 1;
  os.wSuiteMask = 0;
  variant_id = 0;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsWorkStation, osi.id);
  ASSERT_EQ(hhos::kWindowsXP, osi.version);
  ASSERT_EQ(5U, osi.major_version);
  ASSERT_EQ(1U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(hhos::kSp3, osi.sp_version);
  ASSERT_EQ(hhos::kProfessional, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows XP Professional Edition Service Pack 3", osi.pretty_name);
}

TEST(HCOS_UNITTEST, GetOsInfo_2003) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 5;
  os.dwMinorVersion = 2;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 2;
  os.wServicePackMinor = 0;
  os.wSuiteMask = VER_SUITE_WH_SERVER;
  os.wProductType = VER_NT_SERVER;

  uint64_t variant_id = VER_SUITE_WH_SERVER;
  ASSERT_FALSE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                          reinterpret_cast<void *>(&os), variant_id, 0,
                          &osi));

  variant_id = VER_SUITE_DATACENTER;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsServer, osi.id);
  ASSERT_EQ(hhos::kWindowsServer2003, osi.version);
  ASSERT_EQ(5U, osi.major_version);
  ASSERT_EQ(2U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(hhos::kSp2, osi.sp_version);
  ASSERT_EQ(hhos::kDataCenter, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows Server 2003 Datacenter Edition Service Pack 2",
            osi.pretty_name);

  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 1,
                         &osi));
  ASSERT_EQ(hhos::kWindowsServer2003R2, osi.version);
}

TEST(HCOS_UNITTEST, GetOsInfo_Vista) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 6;
  os.dwMinorVersion = 0;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 0;
  os.wServicePackMinor = 0;
  os.wProductType = VER_NT_WORKSTATION;

  uint64_t variant_id = PRODUCT_ULTIMATE;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsWorkStation, osi.id);
  ASSERT_EQ(hhos::kWindowsVista, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(0U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(0U, osi.sp_version);
  ASSERT_EQ(hhos::kUltimate, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows Vista Ultimate Edition", osi.pretty_name);
}

TEST(HCOS_UNITTEST, GetOsInfo_2008) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 6;
  os.dwMinorVersion = 0;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 0;
  os.wServicePackMinor = 0;
  os.wProductType = VER_NT_SERVER;

  uint64_t variant_id = PRODUCT_DATACENTER_SERVER;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsServer, osi.id);
  ASSERT_EQ(hhos::kWindowsServer2008, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(0U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(0U, osi.sp_version);
  ASSERT_EQ(hhos::kDataCenter, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows Server 2008 Datacenter Edition", osi.pretty_name);
}

TEST(HCOS_UNITTEST, GetOsInfo_7) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 6;
  os.dwMinorVersion = 1;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 1;
  os.wServicePackMinor = 0;
  os.wProductType = VER_NT_WORKSTATION;

  uint64_t variant_id = PRODUCT_ULTIMATE;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsWorkStation, osi.id);
  ASSERT_EQ(hhos::kWindows7, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(1U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(hhos::kSp1, osi.sp_version);
  ASSERT_EQ(hhos::kUltimate, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows 7 Ultimate Edition Service Pack 1", osi.pretty_name);
}

TEST(HCOS_UNITTEST, GetOsInfo_2008R2) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 6;
  os.dwMinorVersion = 1;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 0;
  os.wServicePackMinor = 0;
  os.wProductType = VER_NT_SERVER;

  uint64_t variant_id = PRODUCT_DATACENTER_SERVER;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsServer, osi.id);
  ASSERT_EQ(hhos::kWindowsServer2008R2, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(1U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(0U, osi.sp_version);
  ASSERT_EQ(hhos::kDataCenter, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows Server 2008 R2 Datacenter Edition", osi.pretty_name);
}

TEST(HCOS_UNITTEST, GetOsInfo_8) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 6;
  os.dwMinorVersion = 2;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 0;
  os.wServicePackMinor = 0;
  os.wProductType = VER_NT_WORKSTATION;

  uint64_t variant_id = PRODUCT_ENTERPRISE;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsWorkStation, osi.id);
  ASSERT_EQ(hhos::kWindows8, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(2U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(0U, osi.sp_version);
  ASSERT_EQ(hhos::kEnterprise, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows 8 Enterprise Edition", osi.pretty_name);
}

TEST(HCOS_UNITTEST, GetOsInfo_2012) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 6;
  os.dwMinorVersion = 2;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 0;
  os.wServicePackMinor = 0;
  os.wProductType = VER_NT_SERVER;

  uint64_t variant_id = PRODUCT_STANDARD_SERVER;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsServer, osi.id);
  ASSERT_EQ(hhos::kWindowsServer2012, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(2U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(0U, osi.sp_version);
  ASSERT_EQ(hhos::kStandard, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows Server 2012 Standard Edition", osi.pretty_name);
}

TEST(HCOS_UNITTEST, GetOsInfo_8_1) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 6;
  os.dwMinorVersion = 3;
  os.dwBuildNumber = 0;
  os.wServicePackMajor = 0;
  os.wServicePackMinor = 0;
  os.wProductType = VER_NT_WORKSTATION;

  uint64_t variant_id = PRODUCT_ENTERPRISE;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsWorkStation, osi.id);
  ASSERT_EQ(hhos::kWindows8_1, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(3U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(0U, osi.sp_version);
  ASSERT_EQ(hhos::kEnterprise, osi.variant_id);
  ASSERT_EQ(0U, osi.build_id);
  ASSERT_EQ("Windows 8.1 Enterprise Edition", osi.pretty_name);
}

TEST(HCOS_UNITTEST, GetOsInfo_2012R2) {
  hhos::OsIdentification osi;
  SYSTEM_INFO sys_info;
  sys_info.wProcessorArchitecture = PROCESSOR_ARCHITECTURE_AMD64;

  OSVERSIONINFOEX os;
  os.dwMajorVersion = 6;
  os.dwMinorVersion = 3;
  os.dwBuildNumber = 9200;
  os.wServicePackMajor = 0;
  os.wServicePackMinor = 0;
  os.wProductType = VER_NT_SERVER;

  uint64_t variant_id = PRODUCT_STANDARD_SERVER;
  ASSERT_TRUE(_GetOsInfo(reinterpret_cast<void *>(&sys_info),
                         reinterpret_cast<void *>(&os), variant_id, 0,
                         &osi));
  ASSERT_EQ(hhos::kWindowsServer, osi.id);
  ASSERT_EQ(hhos::kWindowsServer2012R2, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(3U, osi.minor_version);
  ASSERT_EQ(64U, osi.arch_id);
  ASSERT_EQ(0U, osi.sp_version);
  ASSERT_EQ(hhos::kStandard, osi.variant_id);
  ASSERT_EQ(9200U, osi.build_id);
  ASSERT_EQ("Windows Server 2012 R2 Standard Edition", osi.pretty_name);
}
#else
#ifdef PLATFORM_64
#define OS_ARCH 64
#define OS_ARCH_DESC "64"
#else
#define OS_ARCH 32
#define OS_ARCH_DESC "32"
#endif

TEST(HCOS_UNITTEST, CentOS5) {
  const std::string centos5_redhat_release("CentOS release 5.11 (Final)");

  hhos::OsIdentification osi;
  ASSERT_TRUE(_GetOsInfo(centos5_redhat_release.c_str(), &osi));
  ASSERT_EQ(hhos::kCentOS, osi.id);
  ASSERT_EQ(hhos::kCentOS5, osi.version);
  ASSERT_EQ(5U, osi.major_version);
  ASSERT_EQ(11U, osi.minor_version);
  ASSERT_EQ("", osi.build_id);
  ASSERT_EQ("final", osi.code_id);
  ASSERT_EQ(static_cast<uint32_t>(OS_ARCH), osi.arch_id);
  ASSERT_EQ(centos5_redhat_release, osi.pretty_name);
}

TEST(HCOS_UNITTEST, CentOS6) {
  const std::string centos6_redhat_release("CentOS release 6.7 (Final)");

  hhos::OsIdentification osi;
  ASSERT_TRUE(_GetOsInfo(centos6_redhat_release.c_str(), &osi));
  ASSERT_EQ(hhos::kCentOS, osi.id);
  ASSERT_EQ(hhos::kCentOS6, osi.version);
  ASSERT_EQ(6U, osi.major_version);
  ASSERT_EQ(7U, osi.minor_version);
  ASSERT_EQ("", osi.build_id);
  ASSERT_EQ("final", osi.code_id);
  ASSERT_EQ(static_cast<uint32_t>(OS_ARCH), osi.arch_id);
  ASSERT_EQ(centos6_redhat_release, osi.pretty_name);
}

TEST(HCOS_UNITTEST, CentOS7) {
  const std::string centos7_redhat_release(
      "CentOS Linux release 7.2.1511 (Core)");

  hhos::OsIdentification osi;
  ASSERT_TRUE(_GetOsInfo(centos7_redhat_release.c_str(), &osi));
  ASSERT_EQ(hhos::kCentOS, osi.id);
  ASSERT_EQ(hhos::kCentOS7, osi.version);
  ASSERT_EQ(7U, osi.major_version);
  ASSERT_EQ(2U, osi.minor_version);
  ASSERT_EQ("1511", osi.build_id);
  ASSERT_EQ("core", osi.code_id);
  ASSERT_EQ(static_cast<uint32_t>(OS_ARCH), osi.arch_id);
  ASSERT_EQ(centos7_redhat_release, osi.pretty_name);
}

TEST(HCOS_UNITTEST, CentOS6_LSB) {
  const std::string lsb_release(
      "LSB_VERSION=base-4.0-amd64"
      ":base-4.0-noarch:core-4.0-amd64"
      ":core-4.0-noarch:graphics-4.0-amd64"
      ":graphics-4.0-noarch"
      ":printing-4.0-amd64"
      ":printing-4.0-noarch");

  hhos::OsIdentification osi;
  ASSERT_FALSE(_GetOsInfo(lsb_release.c_str(), &osi));
}

TEST(HCOS_UNITTEST, Ubuntu_12_04) {
  const std::string lsb_release(
      "DISTRIB_ID=Ubuntu\n"
      "DISTRIB_RELEASE=12.04\n"
      "DISTRIB_CODENAME=precise\n"
      "DISTRIB_DESCRIPTION=\"Ubuntu 12.04 LTS\"");

  hhos::OsIdentification osi;
  ASSERT_TRUE(_GetOsInfo(lsb_release.c_str(), &osi));

  ASSERT_EQ(hhos::kUbuntu, osi.id);
  ASSERT_EQ(hhos::kUbuntu12_04, osi.version);
  EXPECT_EQ(12U, osi.major_version);
  EXPECT_EQ(4U, osi.minor_version);
  EXPECT_EQ("", osi.build_id);
  EXPECT_EQ("precise", osi.code_id);
  EXPECT_EQ(static_cast<uint32_t>(OS_ARCH), osi.arch_id);
  EXPECT_EQ("Ubuntu 12.04 LTS", osi.pretty_name);
}

TEST(HCOS_UNITTEST, CentOS7_OS) {
  const std::string centos7_os_release(
      "NAME=\"CentOS Linux\"\n"
      "VERSION=\"7 (Core)\"\n"
      "ID=\"centos\"\n"
      "ID_LIKE=\"rhel fedora\"\n"
      "VERSION_ID=\"7\"\n"
      "PRETTY_NAME=\"CentOS Linux 7 (Core)\"\n"
      "ANSI_COLOR=\"0;31\"\n"
      "CPE_NAME=\"cpe:/o:centos:centos:7\"\n"
      "HOME_URL=\"https://www.centos.org/\"\n"
      "BUG_REPORT_URL=\"https://bugs.centos.org/\"\n"
      "\n"
      "CENTOS_MANTISBT_PROJECT=\"CentOS-7\"\n"
      "CENTOS_MANTISBT_PROJECT_VERSION=\"7\"\n"
      "REDHAT_SUPPORT_PRODUCT=\"centos\"\n"
      "REDHAT_SUPPORT_PRODUCT_VERSION=\"7\"");

  hhos::OsIdentification osi;
  ASSERT_TRUE(_GetOsInfo(centos7_os_release.c_str(), &osi));

  ASSERT_EQ(hhos::kCentOS, osi.id);
  ASSERT_EQ(hhos::kCentOS7, osi.version);
  ASSERT_EQ(7U, osi.major_version);
  ASSERT_EQ(0U, osi.minor_version);
  ASSERT_EQ("", osi.build_id);
  ASSERT_EQ("", osi.code_id);
  ASSERT_EQ(static_cast<uint32_t>(OS_ARCH), osi.arch_id);
  ASSERT_EQ("CentOS Linux 7 (Core)", osi.pretty_name);
}


TEST(HCOS_UNITTEST, Ubuntu_14_04) {
  const std::string ubuntu_os_release(
      "NAME=\"Ubuntu\"\n"
      "VERSION=\"14.04.4 LTS, Trusty Tahr\"\n"
      "ID=ubuntu\n"
      "ID_LIKE=debian\n"
      "PRETTY_NAME=\"Ubuntu 14.04.4 LTS\"\n"
      "VERSION_ID=\"14.04\"\n"
      "HOME_URL=\"http://www.ubuntu.com/\"\n"
      "SUPPORT_URL=\"http://help.ubuntu.com/\"\n"
      "BUG_REPORT_URL=\"http://bugs.launchpad.net/ubuntu/\"");
  hhos::OsIdentification osi;
  ASSERT_TRUE(_GetOsInfo(ubuntu_os_release.c_str(), &osi));

  ASSERT_EQ(hhos::kUbuntu, osi.id);
  ASSERT_EQ(hhos::kUbuntu14_04, osi.version);
  ASSERT_EQ(14U, osi.major_version);
  ASSERT_EQ(04U, osi.minor_version);
  ASSERT_EQ("", osi.build_id);
  ASSERT_EQ("", osi.code_id);
  ASSERT_EQ(static_cast<uint32_t>(OS_ARCH), osi.arch_id);
  ASSERT_EQ("Ubuntu 14.04.4 LTS", osi.pretty_name);
}
#endif
