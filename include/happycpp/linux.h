﻿// -*- C++ -*-
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

#ifndef INCLUDE_HAPPYCPP_LINUX_H_
#define INCLUDE_HAPPYCPP_LINUX_H_

#include "happycpp/config_platform.h"

#ifndef PLATFORM_WIN32

#include <cstdint>
#include "happycpp/algorithm.h"
#include <string>
#include <vector>
#include <list>

namespace happycpp::hclinux {

    // 判断是否是超级用户运行程序
    HAPPYCPP_SHARED_LIB_API bool isRunSuperUser();

    enum OSType {
        OS_Unknown,
        OS_CentOS,
        OS_RedHat,
        OS_Debian,
        OS_Ubuntu,
    };

    HAPPYCPP_SHARED_LIB_API OSType getOsName();

    // 获取操作系统名称字符串
    HAPPYCPP_SHARED_LIB_API std::string getOsName(const OSType &os);

    namespace hcnet {

        /*
         * 网卡基本信息
         用法演示：
         IfaceFinder finder;
         Iface iface;
         //if(finder.getFrontIface(iface))
         if(finder.getFrontIfaceByName("eth2",iface))
         {
         cout << "name=" << iface.name << endl;
         cout << "ip_addr=" << iface.ip_addr << endl;
         cout << "netmask=" << iface.netmask << endl;
         cout << "gateway=" << iface.gateway << endl;
         cout << "mac=" << iface.mac << endl;
         }
         */
        class Iface {
        public:
            std::string gateway_;
            std::string ip_addr_;
            std::string mac_;
            std::string name_;
            std::string netmask_;
            std::string dns1_;
            std::string dns2_;

            Iface();

            ~Iface();

            /*验证Iface信息*/
            [[nodiscard]] bool verify() const;
        };

        typedef std::vector<Iface> IfaceList;
        typedef std::vector<Iface>::iterator IfaceListIt;

        /*在一个或者多个Iface中，查找指定的Iface或者Iface的类成员对应的值*/
        class IfaceFinder {
        public:
            IfaceFinder();

            ~IfaceFinder();

            std::string getIpAddrByName(const std::string &ifaceName);

            std::string getIpAddrByKeyword(const std::string &keyword);

            bool getIfaceByName(const std::string &ifaceName, Iface *iface);

            bool getFrontIface(Iface *iface);

            bool getBackIface(Iface *iface);

            void getIfaceList(IfaceList *iface_list);

        private:
            enum IFACE_FIELD {
                IFACE_GATEWAY = 0,
                IFACE_IPADDR,
                IFACE_MAC,
                IFACE_NAME,
                IFACE_NETMASK
            };

            IfaceList ifaceList;

            std::string find(const IFACE_FIELD &findField,
                             const IFACE_FIELD &field, const std::string &keyword);

            std::string findByName(const std::string &ifaceName);

            bool getIface(const IfaceListIt &ifaceListIt,
                          const IFACE_FIELD &findField, const std::string &keyword,
                          Iface *iface);

            static std::string getIfaceFieldValue(const IFACE_FIELD &field, const Iface &iface);
        };

        // 获取网卡名称列表
        HAPPYCPP_SHARED_LIB_API bool getIfaceNames(std::vector<std::string> *names);

        // 单网卡绑定多IP时，需要创建网卡别名做为网卡名。比如，eth0的别名
        // eth0:0、eth0:1、eth0:2等等。iface_x_name 只能是标准网卡名称，
        // 比如 eth0 eth1 em0 em1等等。不能是网卡别名。
        // 注意：
        //    使用 NetworkManager 单网卡绑定多IP，不会有类似eth0:0 之类
        // 的虚拟网卡，全部是eth0，这种情况无法支持
        HAPPYCPP_SHARED_LIB_API std::string newAliasForIfaceX(const std::string &iface_x_name);

        // 设置系统DNS，原配置将被清空
        // 修改 /etc/resolv.conf 文件方式
        HAPPYCPP_SHARED_LIB_API bool setupDns(const std::string &dns1, const std::string &dns2);

        namespace hcdebian {

            HAPPYCPP_SHARED_LIB_API bool stopNetwork();

            HAPPYCPP_SHARED_LIB_API bool startNetwork();

            HAPPYCPP_SHARED_LIB_API bool restartNetwork();

            // 更新网卡主IP(主IP不能直接删除，只能更新)
            HAPPYCPP_SHARED_LIB_API bool updatePrimaryIp(Iface *iface);

            // 根据网卡名称，删除网卡次要IP
            // 比如 eth0:1
            HAPPYCPP_SHARED_LIB_API void removeSecondaryIpByName(const std::string &iface_name);

            // 根据IP，删除网卡次要IP
            HAPPYCPP_SHARED_LIB_API void removeSecondaryIp(const std::string &ip_addr);

            // 设置网卡主IP或者从IP，以及DNS
            // 如果指定的网卡名称存在，则覆盖配置
            // 如果指定的网卡名称不存在，则在原配置上追加新配置
            HAPPYCPP_SHARED_LIB_API bool setupIp(Iface *iface);

        }  // namespace hcdebian

        namespace hcredhat {

            HAPPYCPP_SHARED_LIB_API bool stopNetwork();

            HAPPYCPP_SHARED_LIB_API bool startNetwork();

            HAPPYCPP_SHARED_LIB_API bool restartNetwork();

            // 更新网卡主IP(主IP不能直接删除，只能更新)
            HAPPYCPP_SHARED_LIB_API bool updatePrimaryIp(Iface *iface);

            // 根据网卡名称，删除网卡次要IP
            // 比如 eth0:1
            HAPPYCPP_SHARED_LIB_API void removeSecondaryIpByName(const std::string &iface_name);

            // 根据IP，删除网卡次要IP
            HAPPYCPP_SHARED_LIB_API void removeSecondaryIp(const std::string &ip_addr);

            // 设置网卡主IP或者从IP，以及DNS
            // 如果指定的网卡名称存在，则覆盖配置
            // 如果指定的网卡名称不存在，则在原配置上追加新配置
            HAPPYCPP_SHARED_LIB_API bool setupIp(Iface *iface);

        }  // namespace hcredhat

    } /*namespace hcnet*/

    namespace hccpu {

        /* cpu时间，总时间，使用时间，空闲时间 */
        struct CpuTime {
            double total;
            double idle;
            double used;
        };

        /*cpu使用率和空闲率*/
        struct CpuUtil {
            double used;
            char display_used[10];  // 用于显示，usage为10，那display_usage为 10%
            double idle;
            char display_idle[10];
        };

        /*获取cpu型号，不处理重复行，比如
         Quad-Core AMD Opteron(tm) Processor 2350
         Quad-Core AMD Opteron(tm) Processor 2350
         Quad-Core AMD Opteron(tm) Processor 2350
         Quad-Core AMD Opteron(tm) Processor 2350
         Quad-Core AMD Opteron(tm) Processor 2350
         Quad-Core AMD Opteron(tm) Processor 2350
         Quad-Core AMD Opteron(tm) Processor 2350
         Quad-Core AMD Opteron(tm) Processor 2350
         */
        HAPPYCPP_SHARED_LIB_API std::string modelName();

        /* 获取cpu实际核心数，返回uint16_t */
        HAPPYCPP_SHARED_LIB_API uint16_t coreNum();

        /* 获取cpu运行时间相关参数，用于计算cpu使用率 */
        HAPPYCPP_SHARED_LIB_API bool getWorkCpuTime(CpuTime *cpu_time);

        /*获取cpu使用率和空闲率*/
        HAPPYCPP_SHARED_LIB_API void getCpuUtil(CpuUtil *cpu_util);

    } /* namespace hccpu */

    namespace hcmem {

        /* 获取系统总内存，单位MiB */
        HAPPYCPP_SHARED_LIB_API uint32_t totalSysMem();

        /*获取系统空闲内存，单位MiB */
        HAPPYCPP_SHARED_LIB_API uint32_t freeSysMem();

        /*获取系统使用内存，单位MiB */
        HAPPYCPP_SHARED_LIB_API uint32_t useSysMem();

    } /*namespace hcmem*/

    namespace hcswap {

        /*获取系统总虚拟内存，单位MiB */
        HAPPYCPP_SHARED_LIB_API uint32_t totalSysSwap();

        /*获取系统空闲虚拟内存，单位MiB */
        HAPPYCPP_SHARED_LIB_API uint32_t freeSysSwap();

        /*获取系统使用虚拟内存，单位MiB */
        HAPPYCPP_SHARED_LIB_API uint32_t useSysSwap();

    } /*namespace hcswap*/

    /*操作系统相关信息 */
    namespace hcsys {

        /*对服务进程的操作类型 */
        enum ServiceACTION {
            kStop = 0,
            kStart = 1,
            kRestart = 2
        };

        /* 控制服务进程的启动、停止以及重启操作
         * serv_script表示服务进程的管理脚本路径，比如/etc/init.d/mysqld。
         * wait表示是否等待操作完成。
         * 1. wait为true时，表示阻塞式操作服务进程，shell等待操作完毕后，才会返回。
         * 2. wait为false时，表示非阻塞式操作服务进程，
         * shell执行操作命令后，直接返回，不等待。 */
        HAPPYCPP_SHARED_LIB_API bool contrlService(const std::string &serv_script, const ServiceACTION &action,
                                                   bool wait);

        /*获取系统UUID，比如 44454C4C-4600-1056-8050-B7C04F4A3258

         System Information
         Manufacturer: Dell Inc.
         Product Name: PowerEdge SC1435
         Version: Not Specified
         Serial Number: 7FVPJ2X
         UUID: 44454C4C-4600-1056-8050-B7C04F4A3258
         Wake-up Type: Power Switch
         SKU Number: Not Specified
         Family: Not Specified
         */
        HAPPYCPP_SHARED_LIB_API std::string uuid();

        /* 方便显示友好的运行时间信息 */
        struct UPTIME_HUMAN {
            std::string day;
            std::string hour;
            std::string minute;
        };
        typedef UPTIME_HUMAN uptime_t;

        /* How many seconds the system has been running. */
        HAPPYCPP_SHARED_LIB_API time_t UptimeSec();

        HAPPYCPP_SHARED_LIB_API void uptimeHumanReadable(uptime_t *uptime);

        HAPPYCPP_SHARED_LIB_API std::string currentLoad();

        // (后台进程)非阻塞式延时关机，默认为0表示立刻关机
        // 注意:
        //     调用该函数后,主线程必须使用msleep函数等待大于 5秒,
        // 否则主线程退出,将导致子线程强制终止,而失效.
        HAPPYCPP_SHARED_LIB_API bool shutdown();

        // (后台进程)非阻塞式延时重启，默认为0表示立刻重启
        // 注意:
        //     调用该函数后,主线程必须使用msleep函数等待大于 5秒,
        // 否则主线程退出,将导致子线程强制终止,而失效.
        HAPPYCPP_SHARED_LIB_API bool reboot();

    } /*namespace hcsys*/

    namespace hcsocket {

        /*在(SSL_read)读取函数调用之前，获取缓冲区内数据的长度。
         * 未解决的问题：如果单个数据包分次发送，第一个分包大小总是零，原因未知
         * 第二个分包大小正常*/
        HAPPYCPP_SHARED_LIB_API int getSocketSize(int socket);

    } /*namespace hcsocket*/

    namespace hcuser {

        // 错误：返回 -1
        // 成功：返回 用户ID
        HAPPYCPP_SHARED_LIB_API int32_t getUserId(const std::string &usr);

        // 错误：返回 -1
        // 成功：返回 组ID
        HAPPYCPP_SHARED_LIB_API int32_t getGroupId(const std::string &grp);

        // 修改系统账号密码
        HAPPYCPP_SHARED_LIB_API bool changePassword(const std::string &user, const std::string &pwd);

    } /*namespace hcuser*/

    // 锁文件（扩展名为.lock）相关代码
    namespace hclock {

        // 返回.lock文件存放目录
        HAPPYCPP_SHARED_LIB_API std::string getLockFileDir();

        HAPPYCPP_SHARED_LIB_API std::string genLockFileName(const std::string &app, const pid_t &pid = 0);

        HAPPYCPP_SHARED_LIB_API bool crtLockFileByName(const std::string &file_name,
                                                       const std::string &content = "");

        HAPPYCPP_SHARED_LIB_API bool crtLockFileById(const std::string &app, const pid_t &pid = 0,
                                                     const std::string &content = "");

        HAPPYCPP_SHARED_LIB_API bool rmLockFileByName(const std::string &file_name);

        HAPPYCPP_SHARED_LIB_API bool rmLockFileByPid(const std::string &app, const pid_t &pid = 0);

    } /*namespace hclock*/

} /*namespace happycpp*/

#endif  // PLATFORM_WIN32

#endif  // INCLUDE_HAPPYCPP_LINUX_H_
