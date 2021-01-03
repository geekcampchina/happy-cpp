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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_RANDOM_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_RANDOM_H_

#include "happycpp/common.h"
#include <string>

namespace happycpp::hcalgorithm::hcrandom {

    /* 获取随机数
     * 由于是根据时间(单位：纳秒)来生成，所以重复的几率非常小。
     * 来源：linux until 中uuid源代码：gen_uuid.c
     */
    HAPPYCPP_SHARED_LIB_API uint64_t getRandom();

    // 生成随机字符串
    // 默认生成不含空格的，由 ASCII 可见字符组成的字符串
    // kAlnum: 字母以及数字
    // kAlpha: 字母
    // kDigit: 数字
    // kPrint: 可见字符(不包括空格)
    HAPPYCPP_SHARED_LIB_API std::string genRandom(const size_t &size,
                                                  CharClassification = kPrint);

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_RANDOM_H_
