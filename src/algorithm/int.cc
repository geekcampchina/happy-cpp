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

#include "happycpp/algorithm/int.h"
#include <sstream>

using std::ostringstream;
using std::stringstream;
using std::hex;

namespace happycpp::hcalgorithm::hcint {

    HAPPYCPP_SHARED_LIB_API std::string decToHex(const int32_t &n, bool isUppercase) {
        std::stringstream ss;
        if (isUppercase) {
            ss << std::uppercase << hex << n;
        } else {
            ss << hex << n;
        }

        return ss.str();
    }

    HAPPYCPP_SHARED_LIB_API bool isOddNum(const int32_t &n) {
        // 奇数: true
        // 偶数: false
        return (n & 1) != 0;
    }

} /* namespace happycpp */
