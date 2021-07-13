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

#include "happycpp/algorithm/hcstring.h"
#include <boost/algorithm/string.hpp>
#include <iomanip>

namespace happycpp::hcalgorithm::hcstring {

    HAPPYCPP_SHARED_LIB_API bool find(const std::string &s,
                                      const std::string &sub) {
        return (s.size() >= sub.size() && s.find(sub) != std::string::npos);
    }

    HAPPYCPP_SHARED_LIB_API std::string trim(const std::string &s,
                                             const std::string &white_space) {
        const size_t start = s.find_first_not_of(white_space);

        // 字符串只由空白字符组成
        if (start == std::string::npos)
            return "";

        const size_t end = s.find_last_not_of(white_space);

        return s.substr(start, end - start + 1);
    }

    HAPPYCPP_SHARED_LIB_API std::string replace(const std::string &s,
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

    HAPPYCPP_SHARED_LIB_API std::string erase(const std::string &s,
                                              const std::string &sub) {
        std::string str(s);

        if (!s.empty() && !sub.empty() && find(s, sub)) {
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

    HAPPYCPP_SHARED_LIB_API bool isVersion(const std::string &s) {
        if (s.empty())
            return false;

        //  开头和结尾不能是点
        if ((s[0] == '.') || (s[s.size() - 1] == '.'))
            return false;

        for (const auto x : s) { // NOLINT
            if (!isdigit(x) && x != '.')
                return false;
        }

        return true;
    }

    HAPPYCPP_SHARED_LIB_API bool isDigit(const std::string &s) {
        if (s.empty())
            return false;

        for (const auto x : s) { // NOLINT
            if (!isdigit(x))
                return false;
        }

        return true;
    }

    HAPPYCPP_SHARED_LIB_API bool isAlnum(const std::string &s) {
        if (s.empty())
            return false;

        for (const auto x : s) { // NOLINT
            if (!isalnum(x))
                return false;
        }

        return true;
    }

    HAPPYCPP_SHARED_LIB_API bool isAlpha(const std::string &s) {
        if (s.empty())
            return false;

        for (const auto x : s) { // NOLINT
            if (!isalpha(x))
                return false;
        }

        return true;
    }

    HAPPYCPP_SHARED_LIB_API std::string toLower(const std::string &s) {
        std::string str;

        for (const auto x : s)
            str.push_back(tolower(x));

        return str;
    }

    HAPPYCPP_SHARED_LIB_API std::string toUpper(const std::string &s) {
        std::string str;

        for (const auto x : s)
            str.push_back(toupper(x));

        return str;
    }

    /* sep可以是多个分隔符，会按照多分隔符同时分隔 */
    HAPPYCPP_SHARED_LIB_API bool split(const std::string &s, std::vector<std::string> *result,
                                       const std::string &sep) {
        boost::split(*result, s, boost::is_any_of(sep));

        /* 分隔失败，boost会返回数量为1，元素为空的结果 */
        if (result->size() == 1 && (*result->begin()).empty()) {
            result->clear();
            return false;
        }

        return true;
    }

    HAPPYCPP_SHARED_LIB_API void toMap(const std::vector<std::string> &v,
                                       std::map<std::string, std::string> *m) {
        for (const auto &x : v)
            m->insert(std::pair<std::string, std::string>(x, ""));
    }

    HAPPYCPP_SHARED_LIB_API void toMap(const std::string &s,
                                       std::map<std::string, std::string> *m,
                                       const std::string &sep) {
        std::vector<std::string> v;

        split(s, &v);
        toMap(v, m);
    }

    HAPPYCPP_SHARED_LIB_API long toLong(const std::string &s) {
        char *endptr = nullptr;
        const long result = strtol(s.c_str(), &endptr, 10);

        return result;
    }

    HAPPYCPP_SHARED_LIB_API std::string toHexString(const std::string &s,
                                                    const std::string &delimiter,
                                                    bool is_upper_case) {
        std::ostringstream result;

        const size_t sLen = s.size();

        for (size_t i = 0; i < sLen; i++) {
            result << std::hex
                   << std::setfill('0')
                   << std::setw(2)
                   << (is_upper_case ? std::uppercase : std::nouppercase)
                   << int(s[i]);

            if (delimiter.empty() || (i+1) == sLen) {
                continue;
            }

            result << delimiter;
        }

        return result.str();
    }

    HAPPYCPP_SHARED_LIB_API std::string fromHexString(
            const std::string &hexString, uint8_t delimiterSize, bool isShowControlAndSpaceChars) {
        const std::string in = trim(hexString, " ");
        const size_t len = in.length();
        std::string out;

        for (size_t i = 0; i < len; i += (2 + delimiterSize)) {
            size_t tmp;
            std::istringstream iss(in.substr(i, 2));

            iss >> std::hex >> tmp;

            // 如果不显示控制符号和空格，则替换为点
            if (!isShowControlAndSpaceChars && (tmp < 33 || tmp > 255)) {
                // 不可见字符或非ASCII范围默认显示一个点
                tmp = 46;
            }

            out.push_back(static_cast<char>(tmp));
        }

        return out;
    }

} /* namespace happycpp */
