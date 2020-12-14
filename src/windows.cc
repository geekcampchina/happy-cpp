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

#include "happycpp/windows.h"

#ifdef PLATFORM_WIN32

#include "happycpp/algorithm/unit.h"
#include "happycpp/algorithm/time.h"
#include "happycpp/algorithm/double.h"
#include "happycpp/filesys.h"
#include "happycpp/i18n.h"
#include "happycpp/cmd.h"
#include "happycpp/log.h"
#include <cstddef>
// WinSock2.h 和 IPHlpApi.h 必须在 Windows.h 之前包含
#include <WinSock2.h>
#include <IPHlpApi.h>
#include <Windows.h>
#include <lm.h>
#include <comutil.h>
#include <process.h>

using std::to_string;
using happycpp::hccmd::GetExitStatusOfCmd;
using happycpp::hcalgorithm::hctime::HappySleep;
using happycpp::hcalgorithm::hcdouble::Round;
using happycpp::hcalgorithm::hcunit::Convert;

namespace happycpp {

namespace hcwindows {

std::string DwordToStr(const DWORD &dw) {
  const uint32_t i(static_cast<uint32_t>(dw));
  return to_string(i);
}

namespace hcnet {

Iface::Iface()
    : name_(""), mac_(""), ip_addr_netmasks_size_(0), gateways_size_(0),
      dnses_size_(0) {
}

Iface::~Iface() {
}

/*验证Iface信息*/
const bool Iface::Verify() {
  const bool ret = (name_.size() && ip_addr_netmasks_size_ != 0
                    && gateways_size_ != 0 && dnses_size_ != 0);
  return ret;
}

/*获取网卡信息，并将信息填充到ifaces_t*/
class IfaceFiller {
 public:
  explicit IfaceFiller(ifaces_t *ifaces);
  ~IfaceFiller();

 private:
  void Fill(ifaces_t *ifaces);
};

IfaceFiller::IfaceFiller(ifaces_t *ifaces) {
  Fill(ifaces);
}

IfaceFiller::~IfaceFiller() {
}

void IfaceFiller::Fill(ifaces_t *ifaces) {
  std::map<std::string, Iface> iface_map;
  PIP_ADAPTER_ADDRESSES p_addresses = nullptr;
  IP_ADAPTER_DNS_SERVER_ADDRESS *p_dnssrv = nullptr;

  ULONG out_buf_size = 0;
  ::GetAdaptersAddresses(AF_UNSPEC, 0, nullptr, p_addresses, &out_buf_size);
  p_addresses = reinterpret_cast<IP_ADAPTER_ADDRESSES*>(malloc(out_buf_size));

  DWORD aa_ret_val = ::GetAdaptersAddresses(
      AF_INET,
      GAA_FLAG_SKIP_UNICAST | GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST,
      nullptr,
      p_addresses,
      &out_buf_size);

  HAPPY_ASSERT(aa_ret_val == NO_ERROR);

  while (p_addresses) {
    /* 如果不是 An Ethernet network interface 跳过*/
    if (p_addresses->IfType != IF_TYPE_ETHERNET_CSMACD) {
      p_addresses = p_addresses->Next;
      continue;
    }

    const std::string adapter_name = std::string(p_addresses->AdapterName);
    Iface iface;
    iface.name_ = WcsToMbs(p_addresses->FriendlyName);
    p_dnssrv = p_addresses->FirstDnsServerAddress;
    uint32_t dns_num = 0;

    while (p_dnssrv) {
      ++dns_num;
      sockaddr_in *sa_in = reinterpret_cast<sockaddr_in *>(
        p_dnssrv->Address.lpSockaddr);
      const std::string dns = std::string(inet_ntoa(sa_in->sin_addr));

      if (dns_num == 1 || dns_num == 2) {
        iface.dnses_.push_back(dns);
        ++iface.dnses_size_;
      } else {
        break;
      }

      p_dnssrv = p_dnssrv->Next;
    }

    iface_map[adapter_name] = iface;
    p_addresses = p_addresses->Next;
  }  // end of while (p_addresses)

  free(p_addresses);

  // PIP_ADAPTER_INFO结构体指针存储本机网卡信息
  PIP_ADAPTER_INFO p_ip_adapter_info = new IP_ADAPTER_INFO();
  // 得到结构体大小,用于GetAdaptersInfo参数
  ULONG p_ip_adapter_infosize = sizeof(IP_ADAPTER_INFO);
  // 调用GetAdaptersInfo函数,填充pIpAdapterInfo
  // 指针变量。其中stSize参数既是一个输入量也是一个输出量
  ULONG ai_ret_val = ::GetAdaptersInfo(p_ip_adapter_info,
                                       &p_ip_adapter_infosize);

  if (ERROR_BUFFER_OVERFLOW == ai_ret_val) {
    // 如果函数返回的是ERROR_BUFFER_OVERFLOW,
    // 则说明GetAdaptersInfo参数传递的内存空间不够,同时
    // 其传出stSize,表示需要的空间大小。
    // 这也是说明为什么stSize既是一个输入量也是一个输出量。
    // 释放原来的内存空间
    delete p_ip_adapter_info;
    // 重新申请内存空间用来存储所有网卡信息
    p_ip_adapter_info = (PIP_ADAPTER_INFO) new BYTE[p_ip_adapter_infosize];
    // 再次调用GetAdaptersInfo函数,填充pIpAdapterInfo指针变量
    ai_ret_val = GetAdaptersInfo(p_ip_adapter_info, &p_ip_adapter_infosize);
  }

  HAPPY_ASSERT(ERROR_SUCCESS == ai_ret_val);

  while (p_ip_adapter_info) {
    const std::string adapter_name = std::string(
        p_ip_adapter_info->AdapterName);

    if (iface_map.find(adapter_name) == iface_map.end())
      continue;

    Iface *iface = &iface_map[adapter_name];
    std::string mac("");
    char mac_part[40];

    for (UINT i = 0; i < p_ip_adapter_info->AddressLength; i++) {
      if (i == p_ip_adapter_info->AddressLength - 1)
        sprintf_s(mac_part, "%02X", p_ip_adapter_info->Address[i]);
      else
        sprintf_s(mac_part, "%02X-", p_ip_adapter_info->Address[i]);

      mac += mac_part;
    }

    iface->mac_ = mac;
    IP_ADDR_STRING *p_ip_addr = &(p_ip_adapter_info->IpAddressList);
    IP_ADDR_STRING *p_gateway = &(p_ip_adapter_info->GatewayList);

    while (p_ip_addr) {
      const std::string ip_addr = std::string(p_ip_addr->IpAddress.String);
      const std::string netmask = std::string(p_ip_addr->IpMask.String);
      iface->ip_addr_netmasks_[ip_addr] = netmask;
      ++iface->ip_addr_netmasks_size_;
      p_ip_addr = p_ip_addr->Next;
    }

    while (p_gateway) {
      iface->gateways_.push_back(std::string(p_gateway->IpAddress.String));
      ++iface->gateways_size_;
      p_gateway = p_gateway->Next;
    }

    p_ip_adapter_info = p_ip_adapter_info->Next;
  }

  //释放内存空间
  if (p_ip_adapter_info)
    delete p_ip_adapter_info;

  std::map<std::string, Iface>::iterator it;

  for (it = iface_map.begin(); it != iface_map.end(); it++) {
    if (it->second.Verify())
      ifaces->push_back(it->second);
  }
}

IfaceFinder::IfaceFinder() {
  IfaceFiller ifaceFiller(&ifaces_);
}

IfaceFinder::~IfaceFinder() {
}

bool IfaceFinder::GetFrontIface(Iface *iface) {
  if (ifaces_.empty())
    return false;

  *iface = ifaces_.front();
  return true;
}

bool IfaceFinder::GetBackIface(Iface *iface) {
  if (ifaces_.empty())
    return false;

  *iface = ifaces_.back();
  return true;
}

void IfaceFinder::GetIfaces(ifaces_t *ifaces) {
  *ifaces = ifaces_;
}

/* 根据提供的网卡名称，找到网卡并返回网卡上第一个IP */
std::string IfaceFinder::GetIpAddrBbyName(const std::string &name) {
  std::string ip_addr("");
  if (name.empty())
    return ip_addr;

  for (ifaces_it_ = ifaces_.begin(); ifaces_it_ != ifaces_.end();
      ifaces_it_++) {
    if (ifaces_it_->name_ == name) {
      ip_addr = ifaces_it_->ip_addr_netmasks_.begin()->first;
      break;
    }
  }

  return ip_addr;
}

bool IfaceFinder::GetIfaceByName(const std::string &name, Iface *iface) {
  bool ret = false;

  if (name.empty())
    return ret;

  for (ifaces_it_ = ifaces_.begin(); ifaces_it_ != ifaces_.end();
      ifaces_it_++) {
    if (ifaces_it_->name_ == name) {
      *iface = (*ifaces_it_);
      ret = true;
      break;
    }
  }

  return ret;
}

// 设置系统主IP(必须提供IP，子网掩码以及网关)
// 提供的网关是默认路由
bool SetupPrimaryIp(const std::string &name, const std::string &ip,
                    const std::string &mask, const std::string &gateway) {
  if (name.empty() || ip.empty() || mask.empty() || gateway.empty())
    return false;

  // 参数 set 和 source 表示是设置主IP，提供的网关是默认路由
  const std::string cmd("netsh interface ip set address "
                   "name=\"" + name + "\" "
                   "source=static "
                   "addr=" + ip + " "
                   "mask=" + mask + " "
                   "gateway=" + gateway + " "
                   "gwmetric=0");

  return GetExitStatusOfCmd(cmd);
}

// 添加系统次要IP(必须提供IP和子网掩码，网关可选)
// 即使提供网关，也不会是默认路由
bool AddSecondaryIp(const std::string &name, const std::string &ip,
                    const std::string &mask, const std::string &gateway) {
  if (name.empty() || ip.empty() || mask.empty())
    return false;

  // 参数 add 表示设置次要IP，即使提供网关，也不会是默认路由
  std::string cmd("netsh interface ip add address "
             "name=\"" + name + "\" "
             "addr=" + ip + " "
             "mask=" + mask);

  // 设置网关
  if (gateway.size())
    cmd += " gateway=" + gateway + " gwmetric=0";

  return GetExitStatusOfCmd(cmd);
}

// 设置首选DNS
bool SetupPrimaryDns(const std::string &name, const std::string &dns) {
  if (name.empty() || dns.empty())
    return false;

  const std::string cmd("netsh interface ip set dns "
                   "name=\"" + name + "\" "
                   "source=static "
                   "addr=\"" + dns + "\" "
                   "register=primary");

  return GetExitStatusOfCmd(cmd);
}

// 设置备用DNS
bool AddSecondaryDns(const std::string &name, const std::string &dns) {
  if (name.empty() || dns.empty())
    return false;

  const std::string cmd("netsh interface ip add dns "
                   "name=\"" + name + "\" "
                   "addr=\"" + dns + "\"");

  return GetExitStatusOfCmd(cmd);
}

}/*namespace hcnet*/

namespace hccpu {

uint64_t FiletimeToUint64(FILETIME file_time);

uint64_t FiletimeToUint64(FILETIME file_time) {
  ULARGE_INTEGER ui_time;
  ui_time.LowPart = file_time.dwLowDateTime;
  ui_time.HighPart = file_time.dwHighDateTime;
  return ui_time.QuadPart;
}

/* 获取cpu实际核心数，返回uint16_t */
uint16_t CoreNum() {
  SYSTEM_INFO sysInfo;
  ::GetSystemInfo(&sysInfo);

  const DWORD num_of_processors = sysInfo.dwNumberOfProcessors;
  return static_cast<uint16_t>(num_of_processors);
}

/* 获取cpu运行时间相关参数，用于计算cpu使用率 */
bool GetWorkCpuTime(CpuTime *cpu_time) {
  FILETIME kernel_time;
  FILETIME idle_time;
  FILETIME user_time;
  const bool ret_val = ::GetSystemTimes(&idle_time, &kernel_time, &user_time);

  if (!ret_val)
    return ret_val;

  const uint64_t ukernel_time = FiletimeToUint64(kernel_time);
  const uint64_t uidle_time = FiletimeToUint64(idle_time);
  const uint64_t uuser_time = FiletimeToUint64(user_time);

  /* 内核时间已经包含空闲时间 */
  cpu_time->total = ukernel_time + uuser_time;
  cpu_time->idle = uidle_time;
  cpu_time->used = ukernel_time + uuser_time - uidle_time;

  return ret_val;
}

/* 获取cpu使用率和空闲率 */
bool GetCpuUtil(CpuUtil *cpu_util) {
  const int32_t precision = 1;  // 保留一位小数
  CpuTime first_cpu_time;
  CpuTime second_cpu_time;

  /* 为防止失败时，返回空值，所以初始化为0 */
  cpu_util->useage = 0.0;
  cpu_util->idle_rate = 0.0;

  if (!GetWorkCpuTime(&first_cpu_time))
    return false;

  HappySleep(500);

  if (!GetWorkCpuTime(&second_cpu_time))
    return false;

  const uint64_t used_time = second_cpu_time.used - first_cpu_time.used;
  const uint64_t total_time = second_cpu_time.total - first_cpu_time.total;

  double cpu_used = (used_time * 100.00) / total_time;
  cpu_util->useage = Round(cpu_used, precision);

  double cpu_idle = 100 - cpu_used;
  cpu_util->idle_rate = Round(cpu_idle, precision);

  return true;
}

} /* namespace hccpu */

namespace hcmem {

bool GetMemoryInfo(MemoryInfo *mem_info) {
  MEMORYSTATUSEX mem_status;
  /* 必须，否则导致GlobalMemoryStatusEx执行失败 */
  mem_status.dwLength = sizeof(mem_status);

  if (!::GlobalMemoryStatusEx(&mem_status))
    return false;

  const mib_t mem_total_mibs = Convert(kUnitMiB, mem_status.ullTotalPhys);
  const byteSize_t mem_used_bytes =
      mem_status.ullTotalPhys - mem_status.ullAvailPhys;
  const mib_t mem_used_mibs = Convert(kUnitMiB, mem_used_bytes);

  mem_info->total = mem_total_mibs;
  mem_info->free = mem_total_mibs - mem_used_mibs;
  mem_info->used = mem_used_mibs;

  const mib_t swap_total_mibs = Convert(kUnitMiB, mem_status.ullTotalPageFile);
  const byteSize_t swap_used_bytes =
      mem_status.ullTotalPageFile - mem_status.ullAvailPageFile;
  const mib_t swap_used_mibs = Convert(kUnitMiB, swap_used_bytes);

  mem_info->swap_total = swap_total_mibs;
  mem_info->swap_free = swap_total_mibs - swap_used_mibs;
  mem_info->swap_used = swap_used_mibs;

  return true;
}

} /*namespace hcmem*/

namespace hcdrive {

/* 对比盘符，判断是否是系统盘。输入盘符，比如 C:\ 或者 D:\ */
bool IsSysDrive(const std::string &dirve_name);

bool IsSysDrive(const std::string &dirve_name) {
  char buf[MAX_PATH];

  HAPPY_ASSERT(::GetWindowsDirectory(buf, MAX_PATH));

  const bfs::path sys_p(buf);
  const bfs::path p(dirve_name);

  return (sys_p.root_name() == p.root_name());
}

/* 获取驱动器信息列表 */
bool GetDriveInfo(DriveInfoVector *drive_infos) {
  byteSize_t total_bytes = 0;
  byteSize_t free_bytes = 0;
  byteSize_t used_bytes = 0;
  gb_t total_gibs = 0;
  gb_t used_gibs = 0;
  int ret_val = 0;
  char buf[MAX_PATH];
  ULARGE_INTEGER total_num_of_free_bytes;
  ULARGE_INTEGER total_num_of_bytes;
  std::string drive_name("");
  const size_t kNullptrSize = 1;

  ret_val = ::GetLogicalDriveStrings(MAX_PATH, buf);

  if (ret_val == 0)
    return false;

  char *p_buf = buf;
  struct DriveInfo drive_info;

  while (*p_buf) {
    memset(&drive_info, 0, sizeof(drive_info));
    /* 盘符，比如 C:\ 或 D:\ 等等 */
    drive_name = std::string(p_buf);
    const UINT drive_type = ::GetDriveType(p_buf);

    switch (drive_type) {
      case DRIVE_FIXED:
        ret_val = ::GetDiskFreeSpaceEx(p_buf, nullptr, &total_num_of_bytes,
                                       &total_num_of_free_bytes);

        if (ret_val == 0)
          break;

        total_bytes = total_num_of_bytes.QuadPart;
        free_bytes = total_num_of_free_bytes.QuadPart;
        used_bytes = total_bytes - free_bytes;
        total_gibs = Convert(kUnitGiB, total_bytes);
        used_gibs = Convert(kUnitGiB, used_bytes);

        drive_info.volume_label = drive_name;
        drive_info.is_system_drive = IsSysDrive(drive_name);
        drive_info.total = total_gibs;
        drive_info.used = used_gibs;
        drive_info.avail = total_gibs - used_gibs;

        drive_infos->push_back(drive_info);
        break;
      default:
        break;
    }  // end of switch(drive_type)

    p_buf += drive_name.size() + kNullptrSize;
  }  // end of while(*p_buf)

  return true;
}

} /*namespace hcdrive*/

namespace hcsys {

void HappyCreateProcess(const std::string &cmd) {
  PrintLog(LOG_TRACE, "cmd=%s" EOL, cmd.c_str());

  STARTUPINFO si;
  PROCESS_INFORMATION pi;

  ::ZeroMemory(&si, sizeof(si));
  si.cb = sizeof(si);
  ::ZeroMemory(&pi, sizeof(pi));

  const bool ret =
      ::CreateProcess(nullptr,  // No module name (use command line)
                      (LPSTR) cmd.c_str(),  // Command line
                      nullptr,  // Process handle not inheritable
                      nullptr,  // Thread handle not inheritable
                      false,  // Set handle inheritance to false
                      0,  // No creation flags
                      nullptr,  // Use parent's environment block
                      nullptr,  // Use parent's starting directory
                      &si,  // Pointer to STARTUPINFO structure
                      &pi);  // Pointer to PROCESS_INFORMATION structure
  HAPPY_ASSERT(ret);

  /* 0 表示立刻返回，不等待线程结束 */
  ::WaitForSingleObject(pi.hProcess, 0);

  ::CloseHandle(pi.hProcess);
  ::CloseHandle(pi.hThread);
}

enum SYS_ACTION {
  SA_SHUTDOWN,
  SA_REBOOT
};

bool ShutdownOrReboot(const SYS_ACTION &sa, const uint32_t &delay_millisecs) {
  HappySleep(delay_millisecs);
  HANDLE h_token;
  TOKEN_PRIVILEGES tkp;

  std::map<SYS_ACTION, UINT> sa_map;
  sa_map[SA_SHUTDOWN] = EWX_SHUTDOWN;
  sa_map[SA_REBOOT] = EWX_REBOOT;
  HAPPY_ASSERT(sa_map.find(sa) != sa_map.end());

  // Get a token for this process.
  const bool is_get_token(::OpenProcessToken(
    GetCurrentProcess(),
    TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &h_token));
  HAPPY_ASSERT(is_get_token);

  // Get the LUID for the shutdown privilege.
  ::LookupPrivilegeValue(nullptr, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);

  tkp.PrivilegeCount = 1;  // one privilege to set
  tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

  // Get the shutdown privilege for this process.
  ::AdjustTokenPrivileges(h_token, false, &tkp, 0,
      (PTOKEN_PRIVILEGES)(nullptr), 0);
  HAPPY_ASSERT(GetLastError() == ERROR_SUCCESS);

  // Shut down or reboot the system and force all applications to close.
  const bool is_exit(::ExitWindowsEx(sa_map[sa] | EWX_FORCE, 0));
  HAPPY_ASSERT(is_exit);

  return true;
}

void Shutdown() {
  const SYS_ACTION sa(SA_SHUTDOWN);
  ShutdownOrReboot(sa, 10);
}

void Reboot() {
  const SYS_ACTION sa(SA_REBOOT);
  ShutdownOrReboot(sa, 10);
}

unsigned int __stdcall ThrShutdown(LPVOID lpvoid) {
  const uint32_t delay_millisecs(5000);
  const SYS_ACTION sa(SA_SHUTDOWN);
  ShutdownOrReboot(sa, delay_millisecs);
  return 0;
}

unsigned int __stdcall ThrReboot(LPVOID lpvoid) {
  const uint32_t delay_millisecs(5000);
  const SYS_ACTION sa(SA_REBOOT);
  ShutdownOrReboot(sa, delay_millisecs);
  return 0;
}

// (子线程)非阻塞式延时关机，默认为0表示立刻关机
// 安全地关闭windows系统, 支持windows xp/2003/7/2008 等
// 注意:
//     调用该函数后,主线程必须使用HappySleep函数等待大于 5秒,
// 否则主线程退出,将导致子线程强制终止,而失效.
void BgDelayShutdown() {
  HANDLE handle = (HANDLE) _beginthreadex(nullptr, 0, ThrShutdown, 0, 0, 0);
  HAPPY_ASSERT(handle != 0);

  PrintLog(LOG_INFO, "The system is going down for shutdown NOW!%s" EOL);
  ::WaitForSingleObject(handle, 0);
  ::CloseHandle(handle);
}

// (子线程)非阻塞式延时重启，默认为0表示立刻重启
// 安全地重启windows系统, 支持windows xp/2003/7/2008 等
// 注意:
//     调用该函数后,主线程必须使用HappySleep函数等待大于 5秒,
// 否则主线程退出,将导致子线程强制终止,而失效.
void BgDelayReboot() {
  HANDLE handle = (HANDLE) _beginthreadex(nullptr, 0, ThrReboot, 0, 0, 0);
  HAPPY_ASSERT(handle != 0);

  PrintLog(LOG_INFO, "The system is going down for shutdown NOW!" EOL);
  ::WaitForSingleObject(handle, 0);
  ::CloseHandle(handle);
}

} /* namespace hcsys */

namespace hcproc {

// 创建进程锁
// 如果同名进程运行多个实例，返回false
// 否则返回true
bool CreateProcLock(const std::string &proc_name) {
  HANDLE hMutex = ::CreateMutex(nullptr, false, proc_name.c_str());

  if (::GetLastError() == ERROR_ALREADY_EXISTS) {
    ::CloseHandle(hMutex);
    return false;
  }

  return true;
}

} /* namespace hcproc */

// 操作Windows服务相关代码
// service control manager
// 参考：
// System Error Codes
// http://msdn.microsoft.com/en-us/library/windows/desktop/ms681381%28v=vs.85%29.aspx
// 示例：
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb540475%28v=vs.85%29.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb540474%28v=vs.85%29.aspx
// http://msdn.microsoft.com/en-us/library/windows/desktop/bb540473%28v=vs.85%29.aspx
namespace hcscm {
char* g_service_name = nullptr;
HANDLE g_service_stop_event = nullptr;
SERVICE_STATUS g_service_status;
SERVICE_STATUS_HANDLE g_service_status_handle = nullptr;
hcscm::ServiceThreadFunction g_service_thread_function = nullptr;

void ReportServiceStatus(
  DWORD dw_curr_state, DWORD dw_win32_exit_code, DWORD dw_wait_hint) {
  static DWORD dw_check_point_ = 1;
  const bool is_start_pending_(dw_curr_state == SERVICE_START_PENDING);
  const bool is_running_or_stoped_(
    (dw_curr_state == SERVICE_RUNNING) || (dw_curr_state == SERVICE_STOPPED));

  // Fill in the SERVICE_STATUS structure.
  g_service_status.dwCurrentState = dw_curr_state;
  g_service_status.dwWin32ExitCode = dw_win32_exit_code;
  g_service_status.dwWaitHint = dw_wait_hint;
  g_service_status.dwControlsAccepted = (is_start_pending_ ? 0 : SERVICE_ACCEPT_STOP);
  g_service_status.dwCheckPoint = (is_running_or_stoped_ ? 0 : (dw_check_point_++));

  // Report the status of the service to the SCM.
  SetServiceStatus(g_service_status_handle, &g_service_status);
}

void WINAPI ServiceCtrlHandler(DWORD dw_ctrl) {
  // Handle the requested control code.
  switch (dw_ctrl) {
  case SERVICE_CONTROL_STOP:
    ReportServiceStatus(SERVICE_STOP_PENDING, NO_ERROR, 0);
    // Signal the service to stop.
    SetEvent(g_service_stop_event);
    ReportServiceStatus(g_service_status.dwCurrentState, NO_ERROR, 0);
    return;
  case SERVICE_CONTROL_INTERROGATE:
    break;
  default:
    break;
  }
}

void ServiceThread() {
  HAPPY_ASSERT(g_service_thread_function != nullptr);

  DWORD id = 0;
  const HANDLE handle = CreateThread(
    0,                      // default security attributes
    0,                      // use default stack size
    // thread function name
    reinterpret_cast<LPTHREAD_START_ROUTINE>(g_service_thread_function),
    0,                      // argument to thread function
    0,                      // use default creation flags
    &id);              // returns the thread identifier

  HAPPY_ASSERT(handle != nullptr);
}

void ServiceInit(DWORD dw_argc, LPTSTR *lp_argv) {
  // TO_DO: Declare and set any required variables.
  //   Be sure to periodically call ReportServiceStatus() with
  //   SERVICE_START_PENDING. If initialization fails, call
  //   ReportServiceStatus with SERVICE_STOPPED.

  // Create an event. The control handler function, svc_ctrl_handler,
  // signals this event when it receives the stop control code.
  g_service_stop_event = CreateEvent(
    NULL,    // default security attributes
    TRUE,    // manual reset event
    FALSE,   // not signaled
    NULL);   // no name

  HAPPY_ASSERT(g_service_stop_event != nullptr);

  // Report running status when initialization is complete.
  ReportServiceStatus(SERVICE_RUNNING, NO_ERROR, 0);

  // 业务代码
  ServiceThread();

  while (true) {
    // Check whether to stop the service.
    WaitForSingleObject(g_service_stop_event, INFINITE);
    ReportServiceStatus(SERVICE_STOPPED, NO_ERROR, 0);
    return;
  }
}

VOID WINAPI ServiceMain(DWORD dw_argc, LPTSTR *lp_argv) {
  // Register the handler function for the service
  g_service_status_handle =
    RegisterServiceCtrlHandler(g_service_name, ServiceCtrlHandler);

  HAPPY_ASSERT(g_service_status_handle);

  // These SERVICE_STATUS members remain as set here
  g_service_status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
  g_service_status.dwServiceSpecificExitCode = 0;

  // Report initial status to the SCM
  ReportServiceStatus(SERVICE_START_PENDING, NO_ERROR, 3000);

  // Perform service-specific initialization and work.
  ServiceInit(dw_argc, lp_argv);
}

void InitExecuteService(const ExecuteServiceData &esd) {
  HAPPY_ASSERT(!esd.service_name.empty());
  HAPPY_ASSERT(esd.service_thread_function);

  g_service_name = const_cast<char*>(esd.service_name.c_str());
  g_service_thread_function = esd.service_thread_function;
}

void HappyExecuteService(const ExecuteServiceData &esd) {
  InitExecuteService(esd);

  SERVICE_TABLE_ENTRY dispatch_table_[] = {
    { g_service_name, ServiceMain },
    { NULL, NULL }
  };

  HAPPY_ASSERT(StartServiceCtrlDispatcher(dispatch_table_));
}

bool HappyCreateService(const std::string &name, const std::string &display,
                        const std::string &desc) {
  TCHAR sz_path[MAX_PATH];

  // 获取程序绝对路径
  HAPPY_ASSERT(::GetModuleFileName(nullptr, sz_path, MAX_PATH));

  return HappyCreateService(name, sz_path, display, desc);
}

bool HappyCreateService(const std::string &name, const std::string &path,
                        const std::string &display, const std::string &desc) {
  SC_HANDLE sch_scm;
  SC_HANDLE sch_svc;
  SERVICE_DESCRIPTION svc_desc;

  // Get a handle to the SCM database.
  sch_scm = ::OpenSCManager(nullptr,  // local computer
                            nullptr,  // ServicesActive database
                            SC_MANAGER_ALL_ACCESS);  // full access rights
  HAPPY_ASSERT(sch_scm != nullptr);

  // Create the service
  sch_svc = ::CreateService(sch_scm,  // SCM database
                            name.c_str(),  // name of service
                            display.c_str(),  // service name to display
                            SERVICE_ALL_ACCESS,  // desired access
                            SERVICE_WIN32_OWN_PROCESS,  // service type
                            SERVICE_AUTO_START,  // start type
                            SERVICE_ERROR_NORMAL,  // error control type
                            path.c_str(),  // path to service's binary
                            nullptr,  // no load ordering group
                            nullptr,  // no tag identifier
                            nullptr,  // no dependencies
                            nullptr,  // LocalSystem account
                            nullptr);  // no password
  HAPPY_ASSERT(sch_svc != nullptr);

  svc_desc.lpDescription = const_cast<char*>(desc.c_str());
  bool ret = ::ChangeServiceConfig2(
      sch_svc,  // handle to service
      SERVICE_CONFIG_DESCRIPTION,  // change: description
      &svc_desc);
  HAPPY_ASSERT(ret);

  ret = ::ChangeServiceConfig(sch_svc,  // handle of service
                              SERVICE_NO_CHANGE,  // service type: no change
                              SERVICE_AUTO_START,  // service start type
                              SERVICE_NO_CHANGE,  // error control: no change
                              nullptr,  // binary path: no change
                              nullptr,  // load order group: no change
                              nullptr,  // tag ID: no change
                              nullptr,  // dependencies: no change
                              nullptr,  // account name: no change
                              nullptr,  // password: no change
                              nullptr);
  HAPPY_ASSERT(ret);

  ::CloseServiceHandle(sch_svc);
  ::CloseServiceHandle(sch_scm);
  return true;
}

void HappyDeleteService(const std::string &name) {
  SC_HANDLE sch_scm;
  SC_HANDLE sch_svc;

  // Get a handle to the SCM database.
  sch_scm = ::OpenSCManager(nullptr,  // local computer
                            nullptr,  // ServicesActive database
                            SC_MANAGER_ALL_ACCESS);  // full access rights
  HAPPY_ASSERT(sch_scm != nullptr);

  // Get a handle to the service.
  sch_svc = ::OpenService(sch_scm,  // SCM database
                          name.c_str(),  // name of service
                          DELETE);  // need delete access
  HAPPY_ASSERT(sch_svc != nullptr);

  // Delete the service.
  HAPPY_ASSERT(::DeleteService(sch_svc))

  ::CloseServiceHandle(sch_svc);
  ::CloseServiceHandle(sch_scm);
}

bool HappyStartService(const std::string &name) {
  SC_HANDLE sch_scm;
  SC_HANDLE sch_svc;
  SERVICE_STATUS_PROCESS svc_status;
  DWORD dw_old_check_point;
  DWORD dw_start_tick_count;
  DWORD dw_bytes_needed;

  // Get a handle to the SCM database.
  sch_scm = ::OpenSCManager(nullptr,  // local computer
                            nullptr,  // ServicesActive database
                            SC_MANAGER_ALL_ACCESS);  // full access rights
  HAPPY_ASSERT(sch_scm != nullptr);

  // Get a handle to the service.
  sch_svc = ::OpenService(sch_scm,  // SCM database
                          name.c_str(),  // name of service
                          SERVICE_ALL_ACCESS);  // need delete access
  HAPPY_ASSERT(sch_svc != nullptr);

  // Check the status in case the service is not stopped.
  bool ret = ::QueryServiceStatusEx(
      sch_svc,  // handle to service
      SC_STATUS_PROCESS_INFO,  // information level
      (LPBYTE) & svc_status,  // address of structure
      sizeof(SERVICE_STATUS_PROCESS),  // size of structure
      &dw_bytes_needed);  // size needed if buffer is too small
  HAPPY_ASSERT(ret);

  // Check if the service is already running. It would be possible
  // to stop the service here, but for simplicity this example just returns.
  ret = svc_status.dwCurrentState != SERVICE_STOPPED
      && svc_status.dwCurrentState != SERVICE_STOP_PENDING;
  HAPPY_ASSERT(ret);

  // Save the tick count and initial checkpoint.
  dw_start_tick_count = GetTickCount();
  dw_old_check_point = svc_status.dwCheckPoint;

  // Wait for the service to stop before attempting to start it.
  while (svc_status.dwCurrentState == SERVICE_STOP_PENDING) {
    PrintLog(LOG_DEBUG, "Service stop pending..." EOL);
    HappySleep(1000);

    // Check the status until the service is no longer stop pending.
    bool ret = ::QueryServiceStatusEx(
        sch_svc,  // handle to service
        SC_STATUS_PROCESS_INFO,  // information level
        (LPBYTE) & svc_status,  // address of structure
        sizeof(SERVICE_STATUS_PROCESS),  // size of structure
        &dw_bytes_needed);  // size needed if buffer is too small

    HAPPY_ASSERT(ret);

    if (svc_status.dwCheckPoint > dw_old_check_point) {
      // Continue to wait and check.
      dw_start_tick_count = GetTickCount();
      dw_old_check_point = svc_status.dwCheckPoint;
    } else {
      // Timeout waiting for service to stop.
      ret = GetTickCount() - dw_start_tick_count <= svc_status.dwWaitHint;
      HAPPY_ASSERT(ret);
    }
  }

  // Attempt to start the service.
  ret = ::StartService(sch_svc,  // handle to service
                       0,  // number of arguments
                       nullptr);
  HAPPY_ASSERT(ret);

  PrintLog(LOG_DEBUG, "Service start pending..." EOL);

  // Check the status until the service is no longer start pending.
  ret = ::QueryServiceStatusEx(
      sch_svc,  // handle to service
      SC_STATUS_PROCESS_INFO,  // info level
      (LPBYTE) & svc_status,  // address of structure
      sizeof(SERVICE_STATUS_PROCESS),  // size of structure
      &dw_bytes_needed);  // if buffer too small
  HAPPY_ASSERT(ret);

  // Save the tick count and initial checkpoint.
  dw_start_tick_count = GetTickCount();
  dw_old_check_point = svc_status.dwCheckPoint;

  while (svc_status.dwCurrentState == SERVICE_START_PENDING) {
    PrintLog(LOG_DEBUG, "Service stop pending..." EOL);
    HappySleep(1000);

    ret = ::QueryServiceStatusEx(
        sch_svc,  // handle to service
        SC_STATUS_PROCESS_INFO,  // info level
        (LPBYTE) & svc_status,  // address of structure
        sizeof(SERVICE_STATUS_PROCESS),  // size of structure
        &dw_bytes_needed);  // if buffer too small
    HAPPY_ASSERT(ret);

    if (svc_status.dwCheckPoint > dw_old_check_point) {
      // Continue to wait and check.
      dw_start_tick_count = GetTickCount();
      dw_old_check_point = svc_status.dwCheckPoint;
    } else {
      // No progress made within the wait hint.
      ret = GetTickCount() - dw_start_tick_count <= svc_status.dwWaitHint;
      HAPPY_ASSERT(ret);
    }
  }

  // Determine whether the service is running.
  HAPPY_ASSERT(svc_status.dwCurrentState == SERVICE_RUNNING);

  ::CloseServiceHandle(sch_svc);
  ::CloseServiceHandle(sch_scm);
  return false;
}

bool StopDependentService(const SC_HANDLE &sch_scm, const SC_HANDLE &sch_svc);

bool StopDependentService(const SC_HANDLE &sch_scm, const SC_HANDLE &sch_svc) {
  DWORD i;
  DWORD dw_bytes_needed;
  DWORD dw_count;

  LPENUM_SERVICE_STATUS lpDependencies = nullptr;
  ENUM_SERVICE_STATUS ess;
  SC_HANDLE hDepService;
  SERVICE_STATUS_PROCESS svc_status_proc;

  DWORD dw_start_time = GetTickCount();
  DWORD dw_timeout = 30000;  // 30-second time-out

  // Pass a zero-length buffer to get the required buffer size.
  if (::EnumDependentServices(sch_svc, SERVICE_ACTIVE, lpDependencies, 0,
                              &dw_bytes_needed, &dw_count)) {
    // If the Enum call succeeds, then there are no dependent
    // services, so do nothing.
    return true;
  }

  if (GetLastError() != ERROR_MORE_DATA)
    return false;  // Unexpected error

  // Allocate a buffer for the dependencies.
  lpDependencies = reinterpret_cast<LPENUM_SERVICE_STATUS>(
      ::HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dw_bytes_needed));

  if (!lpDependencies)
    return false;

  __try {
    // Enumerate the dependencies.
    if (!::EnumDependentServices(sch_svc, SERVICE_ACTIVE, lpDependencies,
                                 dw_bytes_needed, &dw_bytes_needed, &dw_count))
    return false;

    for (i = 0; i < dw_count; i++) {
      ess = *(lpDependencies + i);
      // Open the service.
      hDepService = ::OpenService(sch_scm, ess.lpServiceName,
                                  SERVICE_STOP | SERVICE_QUERY_STATUS);

      if (!hDepService) return false;

      __try {
        // Send a stop code.
        if (!::ControlService(hDepService, SERVICE_CONTROL_STOP,
                             (LPSERVICE_STATUS)&svc_status_proc))
        return false;

        // Wait for the service to stop.
        while (svc_status_proc.dwCurrentState != SERVICE_STOPPED) {
          HappySleep(1000);

          if (!::QueryServiceStatusEx(hDepService, SC_STATUS_PROCESS_INFO,
                                     (LPBYTE)&svc_status_proc,
                                     sizeof(SERVICE_STATUS_PROCESS),
                                     &dw_bytes_needed))
          return false;

          if (svc_status_proc.dwCurrentState == SERVICE_STOPPED) break;

          if (::GetTickCount() - dw_start_time > dw_timeout) return false;
        }
      }
      __finally {
        // Always release the service handle.
        ::CloseServiceHandle(hDepService);
      }
    }
  }
  __finally {
    // Always free the enumeration buffer.
    ::HeapFree(::GetProcessHeap(), 0, lpDependencies);
  }

  return true;
}

void HappyStopService(const std::string &name) {
  SC_HANDLE sch_scm;
  SC_HANDLE sch_svc;
  SERVICE_STATUS_PROCESS svc_status_proc;
  DWORD dw_start_time = GetTickCount();
  DWORD dw_bytes_needed;
  DWORD dw_timeout = 30000;  // 30-second time-out

  // Get a handle to the SCM database.
  sch_scm = ::OpenSCManager(nullptr,  // local computer
                            nullptr,  // ServicesActive database
                            SC_MANAGER_ALL_ACCESS);  // full access rights
  HAPPY_ASSERT(sch_scm != nullptr);

  // Get a handle to the service.
  sch_svc = ::OpenService(sch_scm,  // SCM database
                          name.c_str(),  // name of service
                          SERVICE_ALL_ACCESS);  // need delete access
  HAPPY_ASSERT(sch_svc != nullptr);

  // Make sure the service is not already stopped.
  bool ret = ::QueryServiceStatusEx(sch_svc, SC_STATUS_PROCESS_INFO,
                                    (LPBYTE) & svc_status_proc,
                                    sizeof(SERVICE_STATUS_PROCESS),
                                    &dw_bytes_needed);
  HAPPY_ASSERT(ret);

  if (svc_status_proc.dwCurrentState == SERVICE_STOPPED) {
    PrintLog(LOG_DEBUG, "Service is already stopped." EOL);
    goto stop_cleanup;
  }

  // If a stop is pending, wait for it.
  while (svc_status_proc.dwCurrentState == SERVICE_STOP_PENDING) {
    PrintLog(LOG_DEBUG, "Service stop pending..." EOL);
    HappySleep(1000);

    ret = ::QueryServiceStatusEx(sch_svc, SC_STATUS_PROCESS_INFO,
                                 (LPBYTE) & svc_status_proc,
                                 sizeof(SERVICE_STATUS_PROCESS),
                                 &dw_bytes_needed);
    HAPPY_ASSERT(ret);

    if (svc_status_proc.dwCurrentState == SERVICE_STOPPED) {
      PrintLog(LOG_DEBUG, "Service stopped successfully." EOL);
      goto stop_cleanup;
    }

    if (::GetTickCount() - dw_start_time > dw_timeout) {
      PrintLog(LOG_DEBUG, "Service stop timeout." EOL);
      goto stop_cleanup;
    }
  }

  // If the service is running, dependencies must be stopped first.
  StopDependentService(sch_scm, sch_svc);

  // Send a stop code to the service.
  ret = ::ControlService(sch_svc, SERVICE_CONTROL_STOP,
                         reinterpret_cast<LPSERVICE_STATUS>(&svc_status_proc));
  HAPPY_ASSERT(ret);

  // Wait for the service to stop.
  while (svc_status_proc.dwCurrentState != SERVICE_STOPPED) {
    PrintLog(LOG_DEBUG, "Service stop pending..." EOL);
    HappySleep(1000);

    ret = ::QueryServiceStatusEx(sch_svc, SC_STATUS_PROCESS_INFO,
                                 (LPBYTE) & svc_status_proc,
                                 sizeof(SERVICE_STATUS_PROCESS),
                                 &dw_bytes_needed);
    HAPPY_ASSERT(ret);

    if (svc_status_proc.dwCurrentState == SERVICE_STOPPED)
      break;

    if ((::GetTickCount() - dw_start_time) > dw_timeout) {
      PrintLog(LOG_FATAL, "Wait timeout." EOL);
      goto stop_cleanup;
    }
  }

stop_cleanup:
  ::CloseServiceHandle(sch_svc);
  ::CloseServiceHandle(sch_scm);
}

} /* namespace hcscm */

namespace hcuser {

// 获取系统用户列表
// 为方便直接查找，使用map
bool ListUsers(std::map<std::string, std::string> *usrs) {
  LPUSER_INFO_0 p_buf = nullptr;
  LPUSER_INFO_0 p_tmp_buf;
  DWORD dw_level = 0;  // Return user account names
  DWORD dw_pref_max_len = MAX_PREFERRED_LENGTH;
  DWORD dw_entries_read = 0;
  DWORD dw_total_entries = 0;
  DWORD dw_resume_handle = 0;
  DWORD i;
  NET_API_STATUS n_status;
  std::string usr_name("");

  usrs->clear();

  do {
    n_status = ::NetUserEnum(
        nullptr,
        dw_level,
        FILTER_NORMAL_ACCOUNT,  // Enumerates normal user account data.
        reinterpret_cast<LPBYTE*>(&p_buf),
        dw_pref_max_len,
        &dw_entries_read,
        &dw_total_entries,
        &dw_resume_handle);

    if ((n_status == NERR_Success) || (n_status == ERROR_MORE_DATA)) {
      if ((p_tmp_buf = p_buf) != nullptr) {
        for (i = 0; (i < dw_total_entries); i++) {
          if (p_tmp_buf == nullptr) {
            PrintLog(
                LOG_FATAL,
                "An access violation has occurred while retrieving information"
                " about user account." EOL);
            break;
          }

          usr_name = WcsToMbs(p_tmp_buf->usri0_name);
          usrs->insert(std::pair<std::string, std::string>(
              usr_name, usr_name));

          p_tmp_buf++;
        }
      }
    } else {
      PrintLog(
          LOG_FATAL,
          "A system error has occurred " "while retrieving information about "
          "user account." EOL);
    }

    if (p_buf != nullptr) {
      ::NetApiBufferFree(p_buf);
      p_buf = nullptr;
    }
  } while (n_status == ERROR_MORE_DATA);

  if (p_buf != nullptr)
    ::NetApiBufferFree(p_buf);

  return (usrs->size() != 0);
}

bool ExistsUser(const std::string &user) {
  if (user.empty())
    return false;

  std::map<std::string, std::string> users;
  ListUsers(&users);

  return (users.find(user) != users.end());
}

bool CreateUser(const std::string &user, const std::string &pwd,
                const std::string &comment, const std::string &local_group) {
  if (user.empty() || pwd.empty()) {
    PrintLog(LOG_ERROR,
             "Sorry, you must enter a valid username and password." EOL);
    return false;
  }

  const _bstr_t usr_bs(_com_util::ConvertStringToBSTR(user.c_str()));
  const _bstr_t pwd_bs(_com_util::ConvertStringToBSTR(pwd.c_str()));

  USER_INFO_1 usr_info;
  DWORD dw_error = 0;
  NET_API_STATUS n_status;

  usr_info.usri1_name = usr_bs;
  usr_info.usri1_password = pwd_bs;
  usr_info.usri1_password_age = 0;
  usr_info.usri1_priv = USER_PRIV_USER;
  usr_info.usri1_home_dir = nullptr;

  if (comment.size()) {
    const _bstr_t comment_bs(_com_util::ConvertStringToBSTR(comment.c_str()));
    usr_info.usri1_comment = comment_bs;
  }

  // 用户不能修改密码、密码不过期
  usr_info.usri1_flags =
      UF_PASSWD_CANT_CHANGE | UF_DONT_EXPIRE_PASSWD | UF_SCRIPT;
  usr_info.usri1_script_path = nullptr;

  n_status = ::NetUserAdd(nullptr, 1, (LPBYTE) & usr_info, &dw_error);

  if (n_status == NERR_Success && local_group.size()) {
    const _bstr_t local_group_bs(_com_util::ConvertStringToBSTR(
        local_group.c_str()));
    _LOCALGROUP_MEMBERS_INFO_3 localgrp_mem_info;
    localgrp_mem_info.lgrmi3_domainandname = usr_info.usri1_name;

    n_status = ::NetLocalGroupAddMembers(nullptr, local_group_bs, 3,
                                         (LPBYTE)(&localgrp_mem_info), 1);
  }

  return (n_status == NERR_Success);
}

// 删除系统用户
bool DeleteUser(const std::string &user) {
  const _bstr_t user_bs(_com_util::ConvertStringToBSTR(user.c_str()));
  NET_API_STATUS n_status = ::NetUserDel(nullptr, user_bs);
  return (n_status == NERR_Success);
}

// 修改系统用户密码
// 账号不存在或修改失败，返回false
bool ChangePassword(const std::string &user, const std::string &pwd) {
  if (user.empty() || pwd.empty()) {
    PrintLog(LOG_ERROR,
             "Sorry, you must enter a valid username and password." EOL);
    return false;
  }

  const _bstr_t user_bs(_com_util::ConvertStringToBSTR(user.c_str()));
  const _bstr_t pwd_bs(_com_util::ConvertStringToBSTR(pwd.c_str()));

  // Specifies a user password
  USER_INFO_1003 usr_info;
  DWORD dw_level = 1003;
  usr_info.usri1003_password = pwd_bs;
  NET_API_STATUS n_status = ::NetUserSetInfo(nullptr, user_bs, dw_level,
                                            (LPBYTE) & usr_info, nullptr);
  return (n_status == NERR_Success);
}

} /* namespace hcuser */

} /* namespace hcwindows */

} /* namespace happycpp */

#endif  // #ifdef PLATFORM_WIN32
