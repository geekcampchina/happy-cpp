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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_STRING_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_STRING_H_

#include "happycpp/common.h"
#include <map>
#include <sstream>
#include <vector>
#include <string>

namespace happycpp {

    namespace hcalgorithm {

        namespace hcstring {

            // 在字符串str中，查找子字符串sub
            HAPPYCPP_SHARED_LIB_API bool find(const std::string &s,
                                              const std::string &sub);

            // 去除首位的空白字符
            HAPPYCPP_SHARED_LIB_API std::string trim(const std::string &s,
                                                     const std::string &white_space = " ");

            // 将字符串中的大写字母转换为小写
            HAPPYCPP_SHARED_LIB_API std::string toLower(const std::string &s);

            // 将字符串中的小写字母转换为大写
            HAPPYCPP_SHARED_LIB_API std::string toUpper(const std::string &s);

            // 替换字符串中所有指定的子字符串
            HAPPYCPP_SHARED_LIB_API std::string replace(const std::string &s,
                                                        const std::string &old_sub,
                                                        const std::string &new_sub);

            // 删除字符串中指定的子字符串
            HAPPYCPP_SHARED_LIB_API std::string erase(const std::string &s,
                                                      const std::string &sub);

            // 判断字符串是否是版本号码
            HAPPYCPP_SHARED_LIB_API bool isVersion(const std::string &s);

            // 判断字符串是否是全数字组成
            HAPPYCPP_SHARED_LIB_API bool isDigit(const std::string &s);

            // 判断字符串是否是字母和数字组成
            HAPPYCPP_SHARED_LIB_API bool isAlnum(const std::string &s);

            // 判断字符串是否是字母组成
            HAPPYCPP_SHARED_LIB_API bool isAlpha(const std::string &s);

            /* sep可以是多个分隔符，会按照多分隔符同时分隔 */
            bool split(const std::string &s, std::vector<std::string> *result,
                       const std::string &sep = "\n");

            // 列表转化为map，key=列表值，value=""
            HAPPYCPP_SHARED_LIB_API void toMap(const std::vector<std::string> &v,
                                               std::map<std::string, std::string> *m);

            /*
             将字符串按分隔符分割，填充到map中
             比如，以\n分割
             a\n
             b\n
             c\n

             map[a]=""
             map[b]=""
             map[c]=""
             */
            HAPPYCPP_SHARED_LIB_API void toMap(const std::string &s,
                                               std::map<std::string, std::string> *m,
                                               const std::string &sep = "\n");

            HAPPYCPP_SHARED_LIB_API long toLong(const std::string &s);

        } /* namespace hcstring */

    } /* namespace hcalgorithm */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_STRING_H_
