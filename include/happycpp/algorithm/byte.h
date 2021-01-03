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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_BYTE_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_BYTE_H_

#include "happycpp/common.h"

namespace happycpp::hcalgorithm::hcbyte {

    // 生成byte数组对应的十六进制字符串
    std::string toHexStringWithDelimiter(const std::vector<byte_t> &bytes, const std::string &delimiter);

    // 生成byte数组对应的十六进制字符串（没有分隔符）
    std::string toHexString(const std::vector<byte_t> &bytes);

    // 生成byte数组对应的十六进制字符串（以空格为分隔符）
    std::string toHexStringWithSpace(const std::vector<byte_t> &bytes);

    // 生成byte数组对应的十六进制字符串（以空格为分隔符），用于日志打印
    std::string toHexStringForPrint(const std::vector<byte_t> &bytes);

    // 十六进制字符串转换为byte数组，十六进制字符串中的指定的分隔符（默认空格）会被自动删除
    std::vector<byte_t> hexStringToBytes(const std::string &s, const std::string &delimiter = " ");

    std::vector<byte_t> to4ByteArray(uint32_t i);

    std::vector<byte_t> to2ByteArray(uint16_t i);

    uint32_t from4ByteArray(const std::vector<byte_t> &bytes);

    uint16_t from2ByteArray(const std::vector<byte_t> &bytes);

} /* namespace happycpp */


#endif  // INCLUDE_HAPPYCPP_ALGORITHM_BYTE_H_
