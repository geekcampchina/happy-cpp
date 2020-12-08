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

#include <happycpp/config_platform.h>

#ifndef PLATFORM_WIN32

#include <pwd.h>
#include <grp.h>
#include <happycpp/linux.h>
#include <happycpp/filesys.h>
#include <happycpp/algorithm/hctime.h>
#include <happycpp/exception.h>
#include <happycpp/cmd.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <net/if.h>
#include <resolv.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <cstring>
#include <fstream>

using happycpp::hcalgorithm::hcarray::exists;
using happycpp::hcalgorithm::hcdouble::round;
using happycpp::hcalgorithm::hcstring::find;
using happycpp::hcalgorithm::hcstring::replace;
using happycpp::hcalgorithm::hcstring::split;
using happycpp::hcalgorithm::hcstring::toLower;
using happycpp::hcalgorithm::hcstring::trim;
using happycpp::hcalgorithm::hctime::happySleep;
using happycpp::hccmd::getExitStatusOfCmd;
using happycpp::hccmd::getOutputOfCmd;

using std::ifstream;
using std::to_string;

namespace happycpp {

    namespace hclinux {

        // 判断是否是超级用户运行程序
        bool isRunSuperUser() {
            return getuid() == 0;
        }

        OSType getOsName() {
            // Ubuntu 12.04.4 LTS \n \l
            // CentOS release 6.5 (Final)
            // Red Hat Enterprise Linux Server release 6.4 (Santiago)
            // Debian GNU/Linux 7.0 \n \l
            const std::string cmd("head -n 1 /etc/issue");
            const std::string issue_desc(toLower(getOutputOfCmd(cmd)));

            if (find(issue_desc, "ubuntu"))
                return OS_Ubuntu;
            else if (find(issue_desc, "debian"))
                return OS_Debian;
            else if (find(issue_desc, "red hat"))
                return OS_RedHat;
            else if (find(issue_desc, "centos"))
                return OS_CentOS;
            else
                return OS_Unknown;
        }

        std::string getOsName(const OSType &os) {
            static std::map<OSType, std::string> os_name_map;
            os_name_map[OS_Ubuntu] = "Ubuntu";
            os_name_map[OS_Debian] = "Debian";
            os_name_map[OS_RedHat] = "Red Hat";
            os_name_map[OS_CentOS] = "CentOS";
            os_name_map[OS_Unknown] = "Unknown";

            return os_name_map[os];
        }

        namespace hcnet {

            Iface::Iface()
                    : gateway_(""),
                      ip_addr_(""),
                      mac_(""),
                      name_(""),
                      netmask_("") {
            }

            Iface::~Iface() {
            }

            // 验证Iface信息
            bool Iface::verify() {
                // 必要的信息，不能为空
                const bool ret = !(name_.empty() || ip_addr_.empty() || netmask_.empty());

                return ret;
            }

            /*获取网卡信息，并将信息填充到IfaceList*/
            class IfaceFiller {
            public:
                explicit IfaceFiller(IfaceList *ifaceList);

                ~IfaceFiller();

            private:
                void getIfaceNum();

                void createSocket(int32_t *sock);

                const int32_t getIfaceList(struct ifconf *ifconf);

                const std::string formatMac(unsigned char *mac_p);

                const std::string getMac(char *name);

                std::string getGateway();

                std::string getNetmask(char *name);

                const std::string getIpAddr(const ifreq &i);

                void getDns(Iface *iface);

                void fill(IfaceList *ifaceList);

            private:
                int32_t ifaces_num_;
                static int32_t kLoopIfaces_;
                static size_t kMacSize_;
                struct ifreq ifreqs_[20];
                struct ifconf ifconf_;
            };

            int32_t IfaceFiller::kLoopIfaces_ = 1;
            size_t IfaceFiller::kMacSize_ = 6;

            IfaceFiller::IfaceFiller(IfaceList *ifaceList) {
                fill(ifaceList);
            }

            IfaceFiller::~IfaceFiller() {
            }

            void IfaceFiller::fill(IfaceList *ifaceList) {
                Iface iface;

                memset(&ifconf_, 0, sizeof(ifconf));
                ifconf_.ifc_buf = reinterpret_cast<char *>(ifreqs_);
                ifconf_.ifc_len = sizeof(ifreqs_);
                getIfaceList(&ifconf_);
                getIfaceNum();

                const std::string gateway = getGateway();
                getDns(&iface);

                // 临时保存网卡名称列表
                std::vector<std::string> tmp_names;

                /*loopIfaces数量为1，所以循环起点为1*/
                const int32_t startIndex = kLoopIfaces_;
                for (int32_t i = startIndex; i <= ifaces_num_; i++) {
                    iface.name_ = ifreqs_[i].ifr_name;

                    // 网卡已经存在，则使用网卡别名，比如eth0:0
                    if (exists(tmp_names, iface.name_))
                        iface.name_ = newAliasForIfaceX(iface.name_);

                    iface.ip_addr_ = getIpAddr(ifreqs_[i]);
                    iface.netmask_ = getNetmask(ifreqs_[i].ifr_name);
                    iface.mac_ = getMac(ifreqs_[i].ifr_name);
                    iface.gateway_ = gateway;

                    // 名称、IP地址以及掩码不能为空，mac和网关可以为空
                    if (iface.verify()) {
                        ifaceList->push_back(iface);
                        tmp_names.push_back(iface.name_);
                    }
                }
            }

            void IfaceFiller::getIfaceNum() {
                /*网卡数量不记录本地loop设备*/
                ifaces_num_ = (ifconf_.ifc_len / sizeof(struct ifreq)) - kLoopIfaces_;
            }

            void IfaceFiller::createSocket(int32_t *sock) {
                /*只处理IPv4，忽略其它*/
                *sock = socket(AF_INET, SOCK_DGRAM, 0);

                HAPPY_ASSERT(*sock >= 0);
            }

            const int32_t IfaceFiller::getIfaceList(struct ifconf *ifconf) {
                int32_t sock, rval;

                createSocket(&sock);
                rval = ioctl(sock, SIOCGIFCONF, reinterpret_cast<char *>(ifconf));

                HAPPY_ASSERT(rval >= 0);
                close(sock);

                return rval;
            }

            const std::string IfaceFiller::formatMac(unsigned char *mac_p) {
                char macBlock[10];
                std::string mac;
                const size_t hex_num_size = sizeof(char) * 2 + 1;

                for (size_t blockIndex = 0; blockIndex < kMacSize_; blockIndex++) {
                    snprintf(macBlock, hex_num_size, "%.2X", mac_p[blockIndex]);

                    if (blockIndex < (kMacSize_ - 1))
                        mac += std::string(macBlock) + ":";
                    else
                        mac += std::string(macBlock);
                }

                return mac;
            }

            const std::string IfaceFiller::getMac(char *name) {
                struct ifreq ifr{};
                int32_t sock;
                unsigned char mac[kMacSize_];

                createSocket(&sock);

                ifr.ifr_addr.sa_family = AF_INET;
                strncpy(ifr.ifr_name, name, IFNAMSIZ - 1);
                const int32_t rval = ioctl(sock, SIOCGIFHWADDR, &ifr);

                HAPPY_ASSERT(rval >= 0);
                memcpy(mac, ifr.ifr_hwaddr.sa_data, kMacSize_);

                close(sock);
                return (formatMac(mac));
            }

            const std::string IfaceFiller::getIpAddr(const ifreq &i) {
                auto *sin = (struct sockaddr_in *) &i.ifr_addr;
                char *ret = inet_ntoa(sin->sin_addr);
                std::string ip;

                if (ret == nullptr)
                    return ip;

                ip = std::string(ret);
                return ip;
            }

            std::string IfaceFiller::getNetmask(char *name) {
                struct ifreq ifr{};
                int32_t sock;
                std::string netmask;

                createSocket(&sock);

                ifr.ifr_addr.sa_family = AF_INET;
                strncpy(ifr.ifr_name, name, IFNAMSIZ - 1);

                const int32_t rval = ioctl(sock, SIOCGIFNETMASK, &ifr);

                HAPPY_ASSERT(rval >= 0);

                struct sockaddr_in sin{};
                memcpy(&sin, &ifr.ifr_addr, sizeof(struct sockaddr));
                char *ret = inet_ntoa(sin.sin_addr);

                HAPPY_ASSERT(ret != nullptr);

                netmask = std::string(ret);
                return netmask;
            }

            std::string IfaceFiller::getGateway() {
                ifstream ihcfilesys;
                ihcfilesys.open("/proc/net/route", ifstream::binary);
                char gateway[16] = "0.0.0.0";
                std::string line;

                if (ihcfilesys.is_open()) {
                    while (getline(ihcfilesys, line)) {
                        uint64_t hex_dest = 0;
                        uint64_t hex_flag = 0;
                        uint64_t hex_gateway = 0;

                        sscanf(line.c_str(), "%*s%lx%lx%lx", &hex_dest, &hex_gateway, &hex_flag);

                        if (hex_flag == 3 && hex_dest == 0 && hex_gateway != 0) {
                            const char *ret = inet_ntop(AF_INET, &hex_gateway, gateway, 16);

                            HAPPY_ASSERT(ret != nullptr);
                            break;
                        }
                    }
                }

                return std::string(gateway);
            }

            void IfaceFiller::getDns(Iface *iface) {
                const int32_t ret_v = res_init();
                if (ret_v != 0)
                    return;

                const uint32_t count(_res.nscount);
                for (uint32_t i = 0; i < count; i++) {
                    char *ret = inet_ntoa(_res.nsaddr_list[i].sin_addr);
                    if (ret == nullptr)
                        continue;

                    if (i == 0 && iface->dns1_.empty())
                        iface->dns1_ = std::string(ret);
                    else if (i == 1 && iface->dns2_.empty())
                        iface->dns2_ = std::string(ret);
                    else
                        break;
                }

                /* 没有设置dns时，系统会返回0.0.0.0，此时设置dns1为空 */
                if (iface->dns1_ == "0.0.0.0"
                    || iface->dns1_ == "127.0.0.0"
                    || iface->dns1_ == "127.0.0.1") {
                    iface->dns1_.clear();
                }
            }

            IfaceFinder::IfaceFinder() {
                IfaceFiller ifaceFiller(&ifaceList);
            }

            IfaceFinder::~IfaceFinder() {
            }

            const bool IfaceFinder::getIface(const IfaceListIt &ifaceListIt,
                                             const IFACE_FIELD &findField,
                                             const std::string &keyword,
                                             Iface *iface) {
                std::string value;

                switch (findField) {
                    case IFACE_GATEWAY:
                        value = (*ifaceListIt).gateway_;
                        break;
                    case IFACE_IPADDR:
                        value = (*ifaceListIt).ip_addr_;
                        break;
                    case IFACE_MAC:
                        value = (*ifaceListIt).mac_;
                        break;
                    case IFACE_NAME:
                        value = (*ifaceListIt).name_;
                        break;
                    case IFACE_NETMASK:
                        value = (*ifaceListIt).netmask_;
                        break;
                    default:
                        break;
                }

                if (value == keyword) {
                    *iface = (*ifaceListIt);
                    return true;
                }

                return false;
            }

            std::string IfaceFinder::getIfaceFieldValue(const IFACE_FIELD &field,
                                                        const Iface &iface) {
                std::string value;

                switch (field) {
                    case IFACE_GATEWAY:
                        value = iface.gateway_;
                        break;
                    case IFACE_IPADDR:
                        value = iface.ip_addr_;
                        break;
                    case IFACE_MAC:
                        value = iface.mac_;
                        break;
                    case IFACE_NAME:
                        value = iface.name_;
                        break;
                    case IFACE_NETMASK:
                        value = iface.netmask_;
                        break;
                    default: HAPPY_ASSERT(false);
                }

                return value;
            }

            std::string IfaceFinder::findByName(const std::string &ifaceName) {
                Iface iface;
                IfaceListIt it;

                for (it = ifaceList.begin(); it < ifaceList.end(); it++) {
                    if (it->name_ == ifaceName) {
                        iface = (*it);
                        break;
                    }
                }

                return getIfaceFieldValue(IFACE_IPADDR, iface);
            }

            std::string IfaceFinder::find(const IFACE_FIELD &findField,
                                          const IFACE_FIELD &field,
                                          const std::string &keyword) {
                Iface iface;
                IfaceListIt it;

                for (it = ifaceList.begin(); it < ifaceList.end(); it++) {
                    if (getIface(it, findField, keyword, &iface)) {
                        break;
                    }
                }

                return getIfaceFieldValue(field, iface);
            }

            /*获取指定网卡的第一个IP*/
            std::string IfaceFinder::getIpAddrByName(const std::string &ifaceName) {
                return findByName(ifaceName);
            }

            /*获取匹配ip关键字的第一个ip*/
            std::string IfaceFinder::getIpAddrByKeyword(const std::string &keyword) {
                return find(IFACE_IPADDR, IFACE_IPADDR, keyword);
            }

            bool IfaceFinder::getIfaceByName(const std::string &ifaceName,
                                             Iface *iface) {
                if (ifaceList.empty()) {
                    return false;
                }

                std::string value;
                IfaceListIt it;

                for (it = ifaceList.begin(); it < ifaceList.end(); it++) {
                    if (getIface(it, IFACE_NAME, ifaceName, iface)) {
                        break;
                    }
                }

                /*验证*/
                if (iface->verify()) {
                    return true;
                }

                return false;
            }

            bool IfaceFinder::getFrontIface(Iface *iface) {
                if (ifaceList.empty())
                    return false;

                *iface = ifaceList.front();

                /*验证*/
                if (iface->verify())
                    return true;

                return false;
            }

            bool IfaceFinder::getBackIface(Iface *iface) {
                if (ifaceList.empty()) {
                    return false;
                }

                *iface = ifaceList.back();

                /*验证*/
                if (iface->verify()) {
                    return true;
                }

                return false;
            }

            void IfaceFinder::getIfaceList(IfaceList *iface_list) {
                *iface_list = ifaceList;
            }

            // 注意：
            //    使用 NetworkManager 单网卡绑定多IP，不会有类似eth0:0 之类的虚拟网卡，
            //    全部是eth0，这种情况无法支持。
            bool getIfaceNames(std::vector<std::string> *names) {
                names->clear();

                const std::string cmd("ifconfig -a|grep 'Link encap'|awk '{print $1}'");
                const std::string devs(getOutputOfCmd(cmd));
                split(devs, names);

                return !names->empty();
            }

            // 单网卡绑定多IP时，需要创建网卡别名做为网卡名。比如，eth0的别名
            // eth0:0、eth0:1、eth0:2 等等。iface_x_name 只能是标准网卡
            // 名称，比如 eth0 eth1 em0 em1等等。不能是网卡别名。
            // 注意：
            //    使用 NetworkManager 单网卡绑定多IP，不会有类似eth0:0 之
            // 类的虚拟网卡，全部是eth0，这种情况无法支持。
            std::string newAliasForIfaceX(const std::string &iface_x_name) {
                std::vector<std::string> old_iface_names;
                const std::string sep(":");
                std::string alias;
                const std::string _iface_x_name(trim(iface_x_name));
                uint32_t iface_num(0);

                getIfaceNames(&old_iface_names);

                while (true) {
                    alias = _iface_x_name + sep + to_string(iface_num);

                    if (exists(old_iface_names, alias)) {
                        ++iface_num;
                        continue;
                    }

                    break;
                }

                return alias;
            }

            // 设置系统DNS，原配置将被清空
            // 修改 /etc/resolv.conf 文件方式
            bool setupDns(const std::string &dns1, const std::string &dns2) {
                const std::string resolv_conf("/etc/resolv.conf");
                const std::string pre_fix("nameserver ");
                std::string content;

                if (!dns1.empty())
                    content = pre_fix + dns1 + "\n";
                if (!dns2.empty())
                    content += pre_fix + dns2 + "\n";

                if (!content.empty()) {
                    hcfilesys::writeFile(resolv_conf, content);
                    return true;
                }

                return false;
            }

            namespace hcdebian {

                bool stopNetwork() {
                    const std::string cmd("/etc/init.d/networking stop");
                    return getExitStatusOfCmd(cmd);
                }

                bool startNetwork() {
                    const std::string cmd("service networking start");
                    return getExitStatusOfCmd(cmd);
                }

                bool RestartNetwork() {
                    // ubuntu 不能直接restart，会提示错误
                    stopNetwork();
                    return startNetwork();
                }

                std::string LoopInterfaceNode() {
                    return std::string("auto lo\n"
                                       "iface lo inet loopback\n"
                                       "\n");
                }

                // 生成单个网卡配置
                std::string GenInterfaceNode(Iface *iface) {
                    // 是否是主IP，网卡名称中不能包含字符 ':'
                    const bool is_primary_ip(!find(iface->name_, ":"));
                    std::string node;

                    node = "auto " + iface->name_ + "\n"
                                                    "iface " + iface->name_ + " inet static\n"
                                                                              "    address " + iface->ip_addr_ + "\n"
                                                                                                                 "    netmask " +
                           iface->netmask_ + "\n";

                    if (is_primary_ip)
                        node += "    gateway " + iface->gateway_ + "\n";

                    node += "\n";

                    return node;
                }

                // 重置所有网卡网络配置(包括系统DNS)
                // 参数 IfaceList，可以使用 IfaceFinder 从内核中获取。
                // 系统中，可能存在 NetworkManager 和 interfaces 配置两种网络管理方式。
                // NetworkManager 只会管理 /etc/network/interfaces 里没配置的网络接口，
                // 所以，直接使用 interfaces 配置即可。
                // 注意：
                //     1. /etc/network/interfaces 中原有配置会被清空
                bool ResetAllInterfacesConf(IfaceList *ifaces) {
                    const std::string interfaces_conf("/etc/network/interfaces");
                    std::string content(LoopInterfaceNode());
                    std::string dns1;
                    std::string dns2;

                    for (auto & iface : *ifaces) {
                        if (dns1.empty())
                            dns1 = iface.dns1_;
                        if (dns2.empty())
                            dns2 = iface.dns2_;

                        content += GenInterfaceNode(&iface);
                    }

                    hcfilesys::writeFile(interfaces_conf, content);
                    setupDns(dns1, dns2);

                    return true;
                }

                // 更新网卡主IP(主IP不能直接删除，只能更新)
                bool updatePrimaryIp(Iface *iface) {
                    if (!iface->verify() || iface->gateway_.empty())
                        return false;

                    // 为 空 或者 找到分号 ":" (网卡次要IP设备名称以分号分割，比如 eth0:1)
                    if (iface->name_.empty() || find(iface->name_, ":"))
                        return false;

                    bool is_update(false);
                    IfaceList ifaces;
                    IfaceFinder finder;
                    finder.getIfaceList(&ifaces);

                    for (auto & it : ifaces) {
                        if (it.name_ == iface->name_) {
                            it.ip_addr_ = iface->ip_addr_;
                            it.netmask_ = iface->netmask_;
                            it.gateway_ = iface->gateway_;
                            if (!iface->dns1_.empty())
                                it.dns1_ = iface->dns1_;
                            if (!iface->dns2_.empty())
                                it.dns2_ = iface->dns2_;
                            is_update = true;
                            break;
                        }
                    }

                    if (is_update)
                        return ResetAllInterfacesConf(&ifaces);

                    return true;
                }

                void removeSecondaryIpByName(const std::string &iface_name) {
                    // 为 空 或者 没有找到分号 ":" (网卡次要IP设备名称以分号分割，比如 eth0:1)
                    if (iface_name.empty() || !find(iface_name, ":"))
                        return;

                    std::vector<std::string> iface_names;
                    getIfaceNames(&iface_names);
                    if (!exists(iface_names, iface_name))
                        return;

                    bool is_update(false);
                    IfaceList ifaces;
                    IfaceFinder finder;
                    finder.getIfaceList(&ifaces);

                    for (auto it = ifaces.begin(); it != ifaces.end(); ++it) {
                        if (it->name_ == iface_name) {
                            ifaces.erase(it);
                            is_update = true;
                            break;
                        }
                    }

                    if (is_update)
                        ResetAllInterfacesConf(&ifaces);
                }

                void removeSecondaryIp(const std::string &ip_addr) {
                    if (ip_addr.empty())
                        return;

                    bool is_update(false);
                    IfaceList ifaces;
                    IfaceFinder finder;
                    finder.getIfaceList(&ifaces);

                    for (auto it = ifaces.begin(); it != ifaces.end(); ++it) {
                        if (it->ip_addr_ == ip_addr) {
                            ifaces.erase(it);
                            is_update = true;
                            break;
                        }
                    }

                    if (is_update)
                        ResetAllInterfacesConf(&ifaces);
                }

                // 设置网卡主IP或者从IP
                // 如果指定的网卡名称存在，则覆盖配置
                // 如果指定的网卡名称不存在，则在原配置上追加新配置
                bool setupIp(Iface *iface) {
                    if (!iface->verify())
                        return false;

                    std::vector<std::string> iface_names;
                    getIfaceNames(&iface_names);
                    const bool is_exist = exists(iface_names, iface->name_);

                    bool is_update(false);
                    IfaceList ifaces;
                    IfaceFinder finder;
                    finder.getIfaceList(&ifaces);

                    if (is_exist) {
                        for (auto & it : ifaces) {
                            if (it.name_ != iface->name_)
                                continue;

                            it.ip_addr_ = iface->ip_addr_;
                            it.netmask_ = iface->netmask_;
                            if (!iface->gateway_.empty())
                                it.gateway_ = iface->gateway_;
                            if (!iface->dns1_.empty())
                                it.dns1_ = iface->dns1_;
                            if (!iface->dns2_.empty())
                                it.dns2_ = iface->dns2_;
                            is_update = true;
                            break;
                        }
                    } else {
                        ifaces.push_back(*iface);
                        is_update = true;
                    }

                    if (is_update)
                        return ResetAllInterfacesConf(&ifaces);

                    return false;
                }

            }  /*namespace hcdebian */

            namespace hcredhat {

                // 创建 或者 覆盖 网络配置文件
                bool ResetNetworkCfg(Iface *iface) {
                    if (!iface->verify())
                        return false;

                    setupDns(iface->dns1_, iface->dns2_);

                    // 是否是主IP，网卡主IP名称中不能包含字符 ':'
                    const bool is_primary_ip(!find(iface->name_, ":"));

                    const std::string cfg_file(
                            "/etc/sysconfig/network-scripts/ifcfg-" + iface->name_);
                    std::string cfg;

                    if (is_primary_ip)
                        cfg = "DEVICE=" + iface->name_ + "\n"
                                                         "BOOTPROTO=static\n"
                                                         "ONBOOT=yes\n"
                                                         "IPADDR=" + iface->ip_addr_ + "\n"
                                                                                       "NETMASK=" + iface->netmask_ +
                              "\n"
                              "GATEWAY=" + iface->gateway_ + "\n";
                    else
                        cfg = "DEVICE=" + iface->name_ + "\n"
                                                         "IPADDR=" + iface->ip_addr_ + "\n"
                                                                                       "NETMASK=" + iface->netmask_ +
                              "\n";

                    hcfilesys::writeFile(cfg_file, cfg);

                    return true;
                }

                bool stopNetwork() {
                    const std::string cmd("service network stop");
                    return getExitStatusOfCmd(cmd);
                }

                bool startNetwork() {
                    const std::string cmd("service network start");
                    return getExitStatusOfCmd(cmd);
                }

                bool restartNetwork() {
                    const std::string cmd("service network restart");
                    return getExitStatusOfCmd(cmd);
                }

                bool setupIp(Iface *iface) {
                    return ResetNetworkCfg(iface);
                }

                // 更新网卡主IP(主IP不能直接删除，只能更新)
                bool updatePrimaryIp(Iface *iface) {
                    if (!iface->verify() || iface->gateway_.empty())
                        return false;

                    // 为 空 或者 找到分号 ":" (网卡次要IP设备名称以分号分割，比如 eth0:1)
                    if (iface->name_.empty() || find(iface->name_, ":"))
                        return false;

                    return ResetNetworkCfg(iface);
                }

                // 根据网卡名称，删除网卡次要IP
                // 比如 eth0:1
                void removeSecondaryIpByName(const std::string &iface_name) {
                    // 为 空 或者 没有找到分号 ":" (网卡次要IP设备名称以分号分割，比如 eth0:1)
                    if (iface_name.empty() || !find(iface_name, ":"))
                        return;

                    const std::string cfg_file(
                            "/etc/sysconfig/network-scripts/ifcfg-" + iface_name);

                    bfs::remove(cfg_file);
                }

                // 根据IP，删除网卡次要IP
                void removeSecondaryIp(const std::string &ip_addr) {
                    if (ip_addr.empty())
                        return;

                    const std::string cmd("egrep -l "
                                          "'^"
                                          "[[:space:]]*IPADDR[[:space:]]*=[[:space:]]*" + ip_addr +
                                          "[[:space:]]*"
                                          "$' "
                                          "/etc/sysconfig/network-scripts/ifcfg-*:*");
                    std::vector<std::string> files;
                    std::vector<std::string>::iterator it;

                    const std::string ret(getOutputOfCmd(cmd));
                    split(ret, &files);

                    // 删除找到的文件
                    for (auto & file : files)
                        bfs::remove(file);
                }

            }  /* namespace hcredhat */

        } /* namespace hcnet */

        namespace hccpu {

            /*
             * 获取cpu型号，不处理重复行，比如
             Quad-Core AMD Opteron(tm) Processor 2350
             Quad-Core AMD Opteron(tm) Processor 2350
             Quad-Core AMD Opteron(tm) Processor 2350
             Quad-Core AMD Opteron(tm) Processor 2350
             Quad-Core AMD Opteron(tm) Processor 2350
             Quad-Core AMD Opteron(tm) Processor 2350
             Quad-Core AMD Opteron(tm) Processor 2350
             Quad-Core AMD Opteron(tm) Processor 2350
             */
            std::string modelName() {
                const std::string cmd(
                        "egrep 'model name' /proc/cpuinfo|awk -F':' '{print $NF}'");
                return getOutputOfCmd(cmd);
            }

            /* 获取cpu实际核心数，返回uint16_t */
            uint16_t coreNum() {
                const std::string cmd("expr `grep 'physical id' /proc/cpuinfo"
                                      "|awk '{print $4}'"
                                      "|sort -u"
                                      "|wc -l` \\* `grep 'cpu cores' /proc/cpuinfo"
                                      "|awk '{print $4}'"
                                      "|sort -u`");
                std::string cpu_num = getOutputOfCmd(cmd);

                uint16_t i_cpu_num = stoi(cpu_num);
                if (i_cpu_num == 0)
                    i_cpu_num = 1;

                return i_cpu_num;
            }

            /* 获取cpu运行时间相关参数，用于计算cpu使用率 */
            bool getWorkCpuTime(CpuTime *cpu_time) {
                std::vector<std::string> cpu_stats;
                std::vector<std::string>::iterator it;

                const std::string cmd(
                        "cat /proc/stat|egrep '^cpu[ ]+'|sed 's/^cpu[ ]\\+//g'");
                const std::string proc_stat = getOutputOfCmd(cmd);

                /*未获取到文件内容*/
                if (proc_stat.empty())
                    return false;

                split(proc_stat, &cpu_stats, " ");

                /*文件内容出乎意外，内容至少是6到9列*/
                if (cpu_stats.size() < 6)
                    return false;

                for (it = cpu_stats.begin(); it != cpu_stats.end(); it++) {
                    cpu_time->total += stod(*it);
                }

                cpu_time->idle = stod(cpu_stats[3]);
                cpu_time->used = cpu_time->total - cpu_time->idle;

                return true;
            }

            /*获取cpu使用率和空闲率*/
            void getCpuUtil(CpuUtil *cpu_util) {
                int32_t precision = 1;  // 保留一位小数
                CpuTime cpu_time_1;
                CpuTime cpu_time_2;

                /*为防止失败时，返回空值，所以初始化为0*/
                cpu_util->used = 0;
                cpu_util->idle = 0;

                if (!getWorkCpuTime(&cpu_time_1))
                    return;

                happySleep(100);  // 延时100毫秒

                if (!getWorkCpuTime(&cpu_time_2))
                    return;

                double used_over_period = cpu_time_2.used - cpu_time_1.used;
                double total_over_period = cpu_time_2.total - cpu_time_1.total;

                double cpu_used = used_over_period / total_over_period * 100;
                cpu_util->used = round(cpu_used, precision);
                snprintf(cpu_util->display_used, sizeof(cpu_util->display_used), "%s%%",
                         to_string(cpu_util->used).c_str());

                double cpu_idle = 100 - cpu_used;
                cpu_util->idle = round(cpu_idle, precision);
                snprintf(cpu_util->display_idle, sizeof(cpu_util->display_idle), "%s%%",
                         to_string(cpu_util->idle).c_str());
            }

        } /* namespace hccpu */

        namespace hcmem {

            /* 获取系统总内存，单位MiB */
            uint32_t totalSysMem() {
                const std::string cmd("free -m |egrep 'Mem:'|awk '{print $2}'");
                std::string memory_size = getOutputOfCmd(cmd);
                return stoi(memory_size);
            }

            /* 获取系统空闲内存，单位MiB */
            uint32_t freeSysMem() {
                const std::string cmd("free -m |egrep 'buffers/cache'|awk '{print $NF}'");
                std::string memory_size = getOutputOfCmd(cmd);
                return stoi(memory_size);
            }

            /* 获取系统使用内存，单位MiB */
            uint32_t useSysMem() {
                const std::string cmd(
                        "free -m |egrep 'buffers/cache'|awk '{print $(NF-1)}'");
                std::string memory_size = getOutputOfCmd(cmd);
                return stoi(memory_size);
            }

        } /*namespace hcmem*/

        namespace hcswap {

            /* 获取系统总虚拟内存，单位MiB */
            uint32_t totalSysSwap() {
                const std::string cmd("free -m|sed -n '4p'|awk '{print $2}'");
                std::string memory_size = getOutputOfCmd(cmd);
                return stoi(memory_size);
            }

            /* 获取系统空闲虚拟内存，单位MiB */
            uint32_t freeSysSwap() {
                const std::string cmd("free -m|sed -n '4p'|awk '{print $4}'");
                std::string memory_size = getOutputOfCmd(cmd);
                return stoi(memory_size);
            }

            /* 获取系统使用虚拟内存，单位MiB */
            uint32_t useSysSwap() {
                const std::string cmd("free -m|sed -n '4p'|awk '{print $3}'");
                std::string memory_size = getOutputOfCmd(cmd);
                return stoi(memory_size);
            }

        } /*namespace hcswap*/

        /* 操作系统相关信息 */
        namespace hcsys {

            /* 控制服务进程的启动、停止以及重启操作
             * serv_script表示服务进程的管理脚本路径，比如/etc/init.d/mysqld。
             * wait表示是否等待操作完成。
             * 1. wait为true时，表示阻塞式操作服务进程，shell等待操作完毕后，才会返回。
             * 2. wait为false时，表示非阻塞式操作服务进程，
             * shell执行操作命令后，直接返回，不等待。 */
            bool contrlService(const std::string &serv_script, const ServiceACTION &action,
                               bool wait) {
                if (bfs::exists(serv_script)) {
                    std::vector<std::string> actions(3);
                    actions[0] = "stop";
                    actions[1] = "start";
                    actions[2] = "restart";
                    std::string cmd(serv_script + actions[action]);

                    if (wait)
                        cmd += " &";

                    return getExitStatusOfCmd(cmd);
                }

                return false;
            }

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
             * */
            std::string uuid() {
                const std::string cmd("dmidecode |egrep 'UUID: '|awk '{print $NF}';");
                return getOutputOfCmd(cmd);
            }

            /* How many seconds the system has been running. */
            time_t UptimeSec();

            /* How many seconds the system has been running. */
            time_t UptimeSec() {
                const std::string proc_file("/proc/uptime");
                const std::string content(hcfilesys::readFile(proc_file));
                std::vector<std::string> cols;
                time_t running_sec = 0;
                split(content, &cols, " ");

                if (cols.size() != 2)
                    return running_sec;

                const double col_1 = stod(cols[0]);
                running_sec = static_cast<time_t>(round(col_1, 0));
                return running_sec;
            }

            void uptimeHumanReadable(uptime_t *uptime) {
                const time_t DAY_SEC = 86400;
                const time_t HOUR_SEC = 3600;
                const time_t MINUTE_SEC = 60;
                const time_t running_sec = UptimeSec();
                time_t no_compute_sec = 0;

                time_t day = running_sec / DAY_SEC;
                uptime->day = to_string(day);
                no_compute_sec = running_sec % DAY_SEC;

                time_t hour = no_compute_sec / HOUR_SEC;
                uptime->hour = to_string(hour);
                no_compute_sec = no_compute_sec % HOUR_SEC;

                time_t minute = no_compute_sec / MINUTE_SEC;
                uptime->minute = to_string(minute);
            }

            std::string currentLoad() {
                const std::string cmd("awk '{print $1}' /proc/loadavg");
                return getOutputOfCmd(cmd);
            }

            bool shutdown() {
                return getExitStatusOfCmd("sleep 5 && shutdown -h now &");
            }

            bool reboot() {
                return getExitStatusOfCmd("sleep 5 && reboot &");
            }

        } /*namespace hcsys*/

        namespace hcsocket {

            /*在(SSL_read)读取函数调用之前，获取缓冲区内数据的长度。
             * 未解决的问题：如果单个数据包分次发送，第一个分包大小总是零，原因未知
             * 第二个分包大小正常*/
            int getSocketSize(int socket) {
                int ires;
                int size = 0;

                ires = ioctl(socket, FIONREAD, &size);

                if (ires != 0)
                    return 0;

                return size;
            }

        } /*namespace hcsocket*/

        namespace hcuser {

            // 错误：返回 -1
            // 成功：返回 用户ID
            int32_t getUserId(const std::string &user) {
                if (user.empty())
                    return -1;

                const int64_t buf_len = sysconf(_SC_GETPW_R_SIZE_MAX);

                if (buf_len == -1)
                    return -1;

                char *buffer = reinterpret_cast<char *>(malloc(sizeof(char) * (buf_len + 1)));
                struct passwd pw_buf{}, *p_pw_buf;

                const int ret = getpwnam_r(user.c_str(), &pw_buf, buffer, buf_len,
                                           &p_pw_buf);
                free(buffer);

                if (0 != ret || !p_pw_buf)
                    return -1;

                return p_pw_buf->pw_uid;
            }

            // 错误：返回 -1
            // 成功：返回 组ID
            int32_t getGroupId(const std::string &group) {
                if (group.empty())
                    return -1;

                const int64_t buf_len = sysconf(_SC_GETGR_R_SIZE_MAX);

                if (buf_len == -1)
                    return -1;

                // requires c99
                char *buffer = reinterpret_cast<char *>(malloc(sizeof(char) * (buf_len + 1)));
                struct group group_buf{}, *p_group_buf;

                const int ret = getgrnam_r(group.c_str(), &group_buf, buffer, buf_len,
                                           &p_group_buf);
                free(buffer);

                if (0 != ret || !p_group_buf)
                    return -1;

                return p_group_buf->gr_gid;
            }

            // 修改系统账号密码
            bool changePassword(const std::string &user, const std::string &pwd) {
                if (user.empty() || pwd.empty())
                    return false;

                std::string format_pwd;
                format_pwd = replace(pwd, "\\", "\\\\");
                format_pwd = replace(format_pwd, "'", "\\'");

                const std::string cmd("echo " + user + ":'" + format_pwd + "'|chpasswd");
                return getExitStatusOfCmd(cmd);
            }

        } /*namespace hcuser*/

        // 锁文件（扩展名为.lock）相关代码
        namespace hclock {
            std::string getLockFileDir() {
                return "/tmp";
            }

            std::string genLockFileName(const std::string &app, const pid_t &pid) {
                const std::string ext(".lock");
                std::string file_name(pid == 0 ?
                                      // 如，a1234.lock 或 diskcache.lock
                                      "/tmp/" + app + ext :
                                      // 如，download.1234.lock 或 diskcache.1234.lock
                                      "/tmp/" + app + "." + to_string(pid) + ext);

                return file_name;
            }

            bool crtLockFileByName(const std::string &file_name,
                                   const std::string &content) {
                hcfilesys::writeFile(file_name, content);
                return true;
            }

            bool crtLockFileById(const std::string &app, const pid_t &pid,
                                 const std::string &content) {
                hcfilesys::writeFile(genLockFileName(app, pid), content);
                return true;
            }

            bool rmLockFileByName(const std::string &file_name) {
                return remove(file_name.c_str()) == 0;
            }

            bool rmLockFileByPid(const std::string &app, const pid_t &pid) {
                return rmLockFileByName(genLockFileName(app, pid));
            }

        } /*namespace hclock*/

    } /*namespace hclinux*/

} /*namespace happycpp */
#endif  // PLATFORM_WIN32

