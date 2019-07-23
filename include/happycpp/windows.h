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

#ifndef INCLUDE_HAPPYCPP_WINDOWS_H_
#define INCLUDE_HAPPYCPP_WINDOWS_H_

#include <happycpp/config_platform.h>

#ifdef PLATFORM_WIN32

#include <stdint.h>
#include <happycpp/type.h>
#include <string>
#include <vector>
#include <map>

namespace happycpp {

namespace hcwindows {

namespace hcnet {

typedef std::vector<std::string> gateways_t;
typedef std::vector<std::string>::iterator gateways_it_t;
typedef std::vector<std::string> dnses_t;
typedef std::vector<std::string>::iterator dnses_it_t;
typedef std::map<std::string, std::string> ip_addr_netmasks_t;
typedef std::map<std::string, std::string>::iterator ip_addr_netmasks_it_t;

// 该类只有数据，建议使用结构体重写
class Iface {
 public:
  Iface();
  ~Iface();

  const bool Verify();

 public:
  std::string name_;
  std::string mac_;
  size_t ip_addr_netmasks_size_;
  size_t gateways_size_;
  size_t dnses_size_;
  ip_addr_netmasks_t ip_addr_netmasks_;
  gateways_t gateways_;
  dnses_t dnses_;
};

typedef std::vector<Iface> ifaces_t;
typedef std::vector<Iface>::iterator ifaces_it_t;

/*在一个或者多个Iface中，查找指定的Iface或者Iface的类成员对应的值*/
class IfaceFinder {
 public:
  IfaceFinder();
  ~IfaceFinder();

  /* 根据提供的网卡名称，找到网卡并返回网卡上第一个IP */
  std::string GetIpAddrBbyName(const std::string &name);
  /*  根据提供的网卡名称，找到网卡并返回Iface对象 */
  bool GetIfaceByName(const std::string &name, Iface *iface);
  /*  返回网卡列表中第一个对象 */
  bool GetFrontIface(Iface *iface);
  /*  返回网卡列表中第后一个对象 */
  bool GetBackIface(Iface *iface);
  /*  返回网卡列表 */
  void GetIfaces(ifaces_t *ifaces);

 private:
  ifaces_t ifaces_;
  ifaces_it_t ifaces_it_;
};

// 设置系统主IP(必须提供IP，子网掩码以及网关)
// 提供的网关是默认路由
bool SetupPrimaryIp(const std::string &name, const std::string &ip,
                    const std::string &mask, const std::string &gateway);

// 添加系统次要IP(必须提供IP和子网掩码，网关可选)
// 即使提供网关，也不会是默认路由
bool AddSecondaryIp(const std::string &name, const std::string &ip,
                    const std::string &mask, const std::string &gateway = "");

// 设置首选DNS
bool SetupPrimaryDns(const std::string &name, const std::string &dns);

// 设置备用DNS
bool AddSecondaryDns(const std::string &name, const std::string &dns);

} /* namespace hcnet */

namespace hccpu {

/* cpu时间，总时间，使用时间，空闲时间 */
struct CpuTime {
  double total;
  double idle;
  double used;
};

/*cpu使用率和空闲率*/
struct CpuUtil {
  double useage;
  double idle_rate;
};

/* 获取cpu实际核心数，返回uint16_t */
uint16_t CoreNum();

/* 获取cpu运行时间相关参数，用于计算cpu使用率 */
bool GetWorkCpuTime(CpuTime *cpu_time);

/* 获取cpu使用率和空闲率 */
bool GetCpuUtil(CpuUtil *cpu_util);

} /* namespace hccpu */

namespace hcmem {

/* 物理内存和虚拟内存信息，单位MiB */
struct MemoryInfo {
  mib_t total;
  mib_t free;
  mib_t used;
  mib_t swap_total;
  mib_t swap_free;
  mib_t swap_used;
};

/* 物理内存和虚拟内存信息，单位MiB */
bool GetMemoryInfo(MemoryInfo *mem_info);

} /* namespace hcmem */

namespace hcdrive {

struct DriveInfo {
  std::string volume_label;  //驱动器标签
  bool is_system_drive;  //是否系统盘
  gib_t total;  //驱动器总大小，单位 GiB
  gib_t used;  //驱动器已用大小，单位 GiB
  gib_t avail;  //驱动器剩余大小，单位 GiB
};

typedef std::vector<DriveInfo> DriveInfoVector;

/* 获取驱动器信息列表 */
bool GetDriveInfo(DriveInfoVector *drive_infos);

} /* namespace hcdrive */

namespace hcsys {

/* 以子进程的方式，在后台执行命令 */
void HappyCreateProcess(const std::string &cmd);

// 安全地关闭windows系统, 支持windows xp/2003/7/2008 等
// 即刻执行,无延迟
void Shutdown();

// 安全地重启windows系统, 支持windows xp/2003/7/2008 等
// 即刻执行,无延迟
void Reboot();

// (子线程)非阻塞式延时关机，默认为0表示立刻关机
// 安全地关闭windows系统, 支持windows xp/2003/7/2008 等
// 注意:
//     调用该函数后,主线程必须使用msleep函数等待大于 5秒,
// 否则主线程退出,将导致子线程强制终止,而失效.
// 不使用参数传递延迟时间是因为在服务进程+子进程+子进程线程中传递的参数失效.
void BgDelayShutdown();

// (子线程)非阻塞式延时重启，默认为0表示立刻重启
// 安全地重启windows系统, 支持windows xp/2003/7/2008 等
// 注意:
//     调用该函数后,主线程必须使用msleep函数等待大于 5秒,
// 否则主线程退出,将导致子线程强制终止,而失效.
// 不使用参数传递延迟时间是因为在服务进程+子进程+子进程线程中传递的参数失效.
void BgDelayReboot();

} /* namespace hcsys */

namespace hcproc {

// 创建进程锁
// 如果同名进程运行多个实例，返回false
// 否则返回true
bool CreateProcLock(const std::string &proc_name);

}

// 操作Windows服务相关代码
// service control manager
namespace hcscm {
typedef uint32_t (*ServiceThreadFunction)();

struct ExecuteServiceData {
  std::string service_name;
  ServiceThreadFunction service_thread_function;
};

void HappyExecuteService(const ExecuteServiceData &esd);

bool HappyCreateService(const std::string &name, const std::string &path,
                        const std::string &display, const std::string &desc);

// 为指定服务程序路径时，默认使用程序运行绝对路径
bool HappyCreateService(const std::string &name, const std::string &display,
                        const std::string &desc);

void HappyDeleteService(const std::string &name);

bool HappyStartService(const std::string &name);

// 超时时间：30秒
void HappyStopService(const std::string &name);

} /* namespace hcscm */

namespace hcuser {

// 获取系统用户列表
// 为方便直接查找，使用map
bool ListUsers(std::map<std::string, std::string> *users);

bool ExistsUser(const std::string &user);

bool CreateUser(const std::string &user, const std::string &pwd,
                const std::string &comment = "",
                const std::string &local_group = "");

bool DeleteUser(const std::string &user);

// 修改系统账号密码
// 账号不存在或修改失败，返回false
bool ChangePassword(const std::string &user, const std::string &pwd);

} /* namespace hcuser */

} /* namespace hcwindows */

} /* namespace happycpp */

#endif  // #ifdef PLATFORM_WIN32

#endif  // INCLUDE_HAPPYCPP_WINDOWS_H_
