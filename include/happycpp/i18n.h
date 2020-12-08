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


/** @file
 *  支持英语和中文UTF-8，其他暂不支持
 *  1. 制作po文件
 *      xgettext --c++ -a test.c -o test.po  --from-code=utf-8
 *  2. 修改生成的 test.po 文件，将CHARSET 换成 UTF-8
 *  3. 再翻译其中的string
 *  4. 创建存放mo文件的目录
 *      mkdir -p locale/zh_CN.utf8/LC_MESSAGES
 *  5. 生成mo文件
 *      msgfmt test.po -o locale/zh_CN.utf8/LC_MESSAGES/test.mo
 *  6. 如果使用zh_CN.utf-8，/usr/lib/locale/zh_CN.utf8/ 必须存在
 */

#include "happycpp/config_platform.h"

#ifndef INCLUDE_HAPPYCPP_I18N_H_
#define INCLUDE_HAPPYCPP_I18N_H_

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <map>

namespace happycpp {

    // 仅仅做类型转换, 转换中文内容有问题
    std::string wcsToMbs(const std::wstring &ws);

    // 仅仅做类型转换, 转换中文内容有问题
    std::wstring mbsToWcs(const std::string &s);

#ifdef PLATFORM_WIN32
    std::string ansiToUtf8(const std::string &s);
    std::string utf8ToAnsi(const std::string &s);
#endif

    std::string _gettext(const std::string &s);

/* 设置当前语言、编码、域(mo名称)和
 * mo文件存放目录(比如, locale/zh_CN/LC_MESSAGES/domainname.mo)
 * 如果指定了空值、不存在或者不支持的locale，则使用系统变量定义的locale
 * locale 为空，则使用当前系统设置的locale值
 * dir_name 为空则使用默认值 locale，比如 locale/zh_CN/LC_MESSAGES/domainname.mo */
    void initGettext(const std::string &locale, const std::string &domain_name,
                     const std::string &dir_name);

}/* namespace happycpp */

#ifdef PLATFORM_WIN32
// native to utf-8
#define _ntu(s) happycpp::ansiToUtf8(s)
// utf-8 to native
#define _utn(s) happycpp::utf8ToAnsi(s)
#else
// native to utf-8
#define _ntu(s) s
// utf-8 to native
#define _utn(s) s
#endif

#define _(txt) happycpp::_gettext(txt)

#endif  // INCLUDE_HAPPYCPP_I18N_H_
