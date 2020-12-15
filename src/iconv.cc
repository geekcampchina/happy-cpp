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
#include "happycpp/iconv.h"
#include <clocale>
#include <iconv.h>
#include "happycpp/log.h"

using namespace std;

namespace happycpp {

    namespace hciconv {
        string getCodeName(LanguageEncodingName_t code, bool isToCode) {
            const string suffix = isToCode ? "//TRANSLIT" : "";

            switch (code) {
                case GB18030:
                    return "GB18030" + suffix;
                case GB2312:
                    return "GB2312" + suffix;
                case GBK:
                    return "GBK" + suffix;
                case UTF8:
                    return "UTF-8" + suffix;
                default:
                    return "";
            }
        }

        string iconvConvert(LanguageEncodingName_t fromCode, LanguageEncodingName_t toCode, const string &s) {
            happycpp::log::HappyLogPtr hlog = happycpp::log::HappyLog::getInstance();

            size_t inSize = s.length();
            char inStr[inSize];
            memcpy(inStr, s.c_str(), inSize);
            char *inStrPtr = inStr;

            size_t outSize = inSize * 4;
            char outStr[outSize];
            memset(outStr, 0, outSize);
            char *outStrPtr = outStr;

            iconv_t conv = iconv_open(getCodeName(toCode, true).c_str(), getCodeName(fromCode).c_str());

            if (conv == (iconv_t) -1) {
                hlog->error("iconv_open函数执行时出错：" + string(strerror(errno)));
                return "";
            }

            if (iconv(conv, &inStrPtr, &inSize, &outStrPtr, &outSize) == (size_t)-1) {
                iconv_close(conv);
                hlog->error("iconv函数执行时出错：" + string(strerror(errno)));
                return "";
            }

            iconv_close(conv);

            return outStr;
        }

    } /* namespace hciconv */

} /* namespace happycpp */
