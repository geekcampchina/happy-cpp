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

#ifndef INCLUDE_HAPPYCPP_ICONV_H_
#define INCLUDE_HAPPYCPP_ICONV_H_

#include "happycpp/config_platform.h"
#include <string>
#include <map>
#include <memory>
#include <vector>

#ifdef PLATFORM_LINUX

namespace happycpp {

    namespace hciconv {

        typedef enum {
            GB2312,
            GBK,
            GB18030,
            UTF8,
        } LanguageEncodingName_t;

        std::string getCodeName(LanguageEncodingName_t code, bool isToCode = false);

        std::string iconvConvert(LanguageEncodingName_t fromCode, LanguageEncodingName_t toCode, const std::string &s);

    } /* namespace hciconv */

} /* namespace happycpp */

#endif

#endif  // INCLUDE_HAPPYCPP_ICONV_H_
