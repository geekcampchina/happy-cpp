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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_UNIT_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_UNIT_H_

#include "happycpp/common.h"
#include <string>
#include <cmath>

namespace happycpp::hcalgorithm::hcunit {

            // 输入二进制词头枚举，输出类似 KiB、MiB、GiB、KB、MB、GB 等单位字符串
            // 输入国际单位制词头枚举，输出类似 KiB、MiB、GiB、KB、MB、GB 等单位字符串
            HAPPYCPP_SHARED_LIB_API std::string toStr(UnitType ut);

            HAPPYCPP_SHARED_LIB_API byteSize_t toByte(UnitType ut);

            // 将指定进制单位值转换为 byteSize_t
            HAPPYCPP_SHARED_LIB_API byteSize_t toByte(UnitType ut, double s);

            // 将 byteSize_t 转换为指定进制单位值
            HAPPYCPP_SHARED_LIB_API double convert(UnitType ut, byteSize_t b,
                                                   uint32_t precision = 2);

        } /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_UNIT_H_
