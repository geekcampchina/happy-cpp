﻿// Copyright (c) 2016, Fifi Lyu. All rights reserved.
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

#include "happycpp/algorithm/hcstring.h"
#include <boost/algorithm/string.hpp>

namespace happycpp {

    namespace hcalgorithm {

        namespace hcstring {

            HAPPYCPP_SHARED_LIB_API bool Find(const std::string &s,
                                              const std::string &sub) {
                return (s.size() >= sub.size() && s.find(sub) != std::string::npos);
            }

            HAPPYCPP_SHARED_LIB_API std::string Trim(const std::string &s,
                                                     const std::string &white_space) {
                const size_t start = s.find_first_not_of(white_space);

                // 字符串只由空白字符组成
                if (start == std::string::npos)
                    return "";

                const size_t end = s.find_last_not_of(white_space);

                return s.substr(start, end - start + 1);
            }

            HAPPYCPP_SHARED_LIB_API std::string Replace(const std::string &s,
                                                        const std::string &old_sub,
                                                        const std::string &new_sub) {
                if (s.empty())
                    return "";

                if (old_sub.empty() || old_sub == new_sub)
                    return s;

                size_t pos = 0;
                std::string str(s);

                // find(old_sub,pos)防止出现man替换为woman之类的情况，出现死循环
                while ((pos = str.find(old_sub, pos)) != std::string::npos) {
                    str.replace(pos, old_sub.size(), new_sub);
                    pos += new_sub.size();
                }

                return str;
            }

            HAPPYCPP_SHARED_LIB_API std::string Erase(const std::string &s,
                                                      const std::string &sub) {
                std::string str(s);

                if (!s.empty() && !sub.empty() && Find(s, sub)) {
                    size_t pos(0);
                    size_t erase_pos(0);
                    size_t sub_size = sub.size();

                    // 注意：此代码将防止出现mnmmnn中2次删除mn，剩下第三位的m
                    // 和最后的n时， 多执行了一次删除
                    while ((pos = s.find(sub, pos)) != std::string::npos) {
                        // 在新字符串长度比输入的字符串长度短erase_pos_
                        // 所以，在新字符串中起点向后移动erase_pos_
                        str.erase((pos - erase_pos), sub_size);
                        erase_pos += sub_size;
                        pos += sub.size();
                    }
                }

                return str;
            }

            HAPPYCPP_SHARED_LIB_API bool IsVersion(const std::string &s) {
                if (s.empty())
                    return false;

                //  开头和结尾不能是点
                if ((s[0] == '.') || (s[s.size() - 1] == '.'))
                    return false;

                for (const auto x : s) {
                    if (!isdigit(x) && x != '.')
                        return false;
                }

                return true;
            }

            HAPPYCPP_SHARED_LIB_API bool IsDigit(const std::string &s) {
                if (s.empty())
                    return false;

                for (const auto x : s) {
                    if (!isdigit(x))
                        return false;
                }

                return true;
            }

            HAPPYCPP_SHARED_LIB_API bool IsAlnum(const std::string &s) {
                if (s.empty())
                    return false;

                for (const auto x : s) {
                    if (!isalnum(x))
                        return false;
                }

                return true;
            }

            HAPPYCPP_SHARED_LIB_API bool IsAlpha(const std::string &s) {
                if (s.empty())
                    return false;

                for (const auto x : s) {
                    if (!isalpha(x))
                        return false;
                }

                return true;
            }

            HAPPYCPP_SHARED_LIB_API std::string ToLower(const std::string &s) {
                std::string str;

                for (const auto x : s)
                    str.push_back(tolower(x));

                return str;
            }

            HAPPYCPP_SHARED_LIB_API std::string ToUpper(const std::string &s) {
                std::string str;

                for (const auto x : s)
                    str.push_back(toupper(x));

                return str;
            }

/* sep可以是多个分隔符，会按照多分隔符同时分隔 */
            bool Split(const std::string &s, std::vector<std::string> *result,
                       const std::string &sep) {
                boost::split(*result, s, boost::is_any_of(sep));

                /* 分隔失败，boost会返回数量为1，元素为空的结果 */
                if (result->size() == 1 && (*result->begin()).empty()) {
                    result->clear();
                    return false;
                }

                return true;
            }

            HAPPYCPP_SHARED_LIB_API void ToMap(const std::vector<std::string> &v,
                                               std::map<std::string, std::string> *m) {
                for (const auto& x : v)
                    m->insert(std::pair<std::string, std::string>(x, ""));
            }

            HAPPYCPP_SHARED_LIB_API void ToMap(const std::string &s,
                                               std::map<std::string, std::string> *m,
                                               const std::string &sep) {
                std::vector<std::string> v;

                Split(s, &v);
                ToMap(v, m);
            }

        } /* namespace hcstring */

    } /* namespace hcalgorithm */

} /* namespace happycpp */