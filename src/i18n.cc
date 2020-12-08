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

#include "happycpp/common.h"
#include "happycpp/i18n.h"
#include <clocale>

#ifdef PLATFORM_WIN32
#include <Windows.h>
#endif

#include <libintl.h>
#include <vector>

namespace happycpp {

    std::string wcsToMbs(const std::wstring &ws) {
        const std::string locale = setlocale(LC_ALL, "");
        const wchar_t *wch_src = ws.c_str();
        const size_t dest_size = wcstombs(nullptr, wch_src, 0);

        char *ch_dest = new char[dest_size];
        memset(ch_dest, 0, dest_size);

        wcstombs(ch_dest, wch_src, dest_size);
        std::string s(ch_dest, dest_size);

        delete[] ch_dest;
        setlocale(LC_ALL, locale.c_str());

        return s;
    }

    std::wstring mbsToWcs(const std::string &s) {
        const std::string locale = setlocale(LC_ALL, "");
        const char *ch_src = s.c_str();

        size_t dest_size = mbstowcs(nullptr, ch_src, 0);
        auto *wch_dest = new wchar_t[dest_size];
        wmemset(wch_dest, 0, dest_size);

        mbstowcs(wch_dest, ch_src, dest_size);
        std::wstring ws(wch_dest, dest_size);

        delete[] wch_dest;
        setlocale(LC_ALL, locale.c_str());

        return ws;
    }

#ifdef PLATFORM_WIN32
    std::wstring codePageToUnicode(std::string src, const uint32_t code_page) {
      const size_t src_size = src.size();
      const size_t dest_size = MultiByteToWideChar(code_page,
        0,
        src.c_str(),
        src_size,
        0,
        0);

      std::vector<wchar_t> buffer(dest_size + 1);
      buffer[dest_size] = '\0';

      const int ret_val = MultiByteToWideChar(code_page,
        0,
        src.c_str(),
        src_size,
        &buffer[0],
        dest_size);

      return ret_val == 0 ? L"" : &buffer[0];
    }

    std::string unicodeToCodePage(std::wstring src, const uint32_t code_page) {
      const size_t src_size = src.size();
      const size_t dest_size = ::WideCharToMultiByte(code_page,
        0,
        src.c_str(),
        src_size,
        nullptr,
        0,
        nullptr,
        nullptr);

      std::vector<char> buffer(dest_size + 1);
      buffer[dest_size] = '\0';

      const int ret_val = ::WideCharToMultiByte(code_page,
        0,
        src.c_str(),
        src_size,
        &buffer[0],
        dest_size,
        nullptr,
        nullptr);

      return ret_val == 0 ? "" : &buffer[0];
    }

    std::string ansiToUtf8(const std::string &s) {
      const std::wstring ws(codePageToUnicode(s, CP_ACP));
      return unicodeToCodePage(ws, CP_UTF8);
    }

    std::string utf8ToAnsi(const std::string &s) {
      const std::wstring ws(codePageToUnicode(s, CP_UTF8));
      return unicodeToCodePage(ws, CP_ACP);
    }
#endif

    std::string _gettext(const std::string &s) {
        return std::string(gettext(s.c_str()));
    }

/* 设置当前语言、编码、域(mo名称)和
 * mo文件存放目录(比如, locale/zh_CN/LC_MESSAGES/domainname.mo)
 * 如果指定了空值、不存在或者不支持的locale，则使用系统变量定义的locale
 * locale 为空，则使用当前系统设置的locale值
 * dir_name 为空则使用默认值 locale，比如 locale/zh_CN/LC_MESSAGES/domainname.mo */
    void initGettext(const std::string &locale, const std::string &domain_name,
                     const std::string &dir_name) {
        /* 为空则使用默认值 locale */
        const std::string _dir_name(dir_name.empty() ? "locale" : dir_name);

        /* 设置当前语言，如果为空，则从系统环境变量读取 */
        HAPPY_ASSERT(setlocale(LC_ALL, locale.c_str()) != nullptr);

        // mo文件存放的文件夹，完整的路径是：dirname/locale/category/domainname.mo
        // 比如 locale/zh_CN/LC_MESSAGES/domainname.mo
        HAPPY_ASSERT(bindtextdomain(domain_name.c_str(),
                                    _dir_name.c_str()) != nullptr);

        HAPPY_ASSERT(textdomain(domain_name.c_str()) != nullptr);
    }

} /* namespace happycpp */
