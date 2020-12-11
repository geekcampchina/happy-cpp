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

#include "happycpp/algorithm/byte.h"
#include <boost/algorithm/string.hpp>
#include "happycpp/algorithm/hcstring.h"
#include <sstream>
#include <iomanip>

using namespace std;

namespace happycpp {

    namespace hcalgorithm {

        namespace hcbyte {

            template<typename T>
            string intToHex(T i) {
                stringstream stream;
                stream << setfill('0') << setw(sizeof(T) * 2)
                       << hex << i;
                return stream.str();
            }

            // 生成byte数组对应的十六进制字符串
            string toHexStringWithDelimiter(const vector<byte_t> &bytes, const string &delimiter) {
                string hexStr;
                string hexTmpStr;

                for (byte_t b : bytes) {
                    hexTmpStr = intToHex(b);

//                    if (hexTmpStr.length() == 1) {
//                        hexStr.append("0");
//                    }

                    boost::to_upper(hexTmpStr);
                    hexStr.append(hexTmpStr);

                    if (!delimiter.empty()) {
                        hexStr.append(delimiter);
                    }
                }

                if (!hexStr.empty() && !delimiter.empty()) {
                    const size_t delimiterLen = delimiter.size();
                    // 移除最后一个分隔符
                    hexStr.erase(hexStr.size() - delimiterLen, delimiterLen);
                }

                return hexStr;
            }

            // 生成byte数组对应的十六进制字符串（没有分隔符）
            string toHexString(const vector<byte_t> &bytes) {
                return toHexStringWithDelimiter(bytes, "");
            }

            // 生成byte数组对应的十六进制字符串（以空格为分隔符）
            string toHexStringWithSpace(const vector<byte_t> &bytes) {
                return toHexStringWithDelimiter(bytes, " ");
            }

            // 生成byte数组对应的十六进制字符串（以空格为分隔符），用于日志打印
            string toHexStringForPrint(const vector<byte_t> &bytes) {
                return "[" + toHexStringWithDelimiter(bytes, ", ") + "]";
            }

            // 十六进制字符串转换为byte数组，十六进制字符串中的空格会被自动删除
            vector<byte_t> hexStringToBytes(const string &s) {
                const string _s = hcstring::trim(s, " ");
                const int hexStrLen = s.length();

                vector<byte_t> bytes(0);

                if (hexStrLen < 2) {
                    return bytes;
                }

                if ((hexStrLen % 2) != 0) {
                    // 长度不是偶数
                    return bytes;
                }

                bytes.resize(hexStrLen / 2);

                for (size_t i = 0; i < bytes.size(); i++) {
                    const size_t index = i * 2;
                    const uint32_t j = stoul(_s.substr(index, index + 2), nullptr, 16);
                    bytes[i] = j;
                }

                return bytes;
            }

        } /* namespace hcbyte */

    } /* namespace hcalgorithm */

} /* namespace happycpp */