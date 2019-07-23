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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_DOUBLE_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_DOUBLE_H_

#include <happycpp/common.h>

namespace happycpp {

namespace hcalgorithm {

namespace hcdouble {

/*
 四舍五入浮点数
 d 为原来的值
 precision 为要保留的小数个数，也就是精度

 返回值：类似
 传入4.9999，返回5
 传入4.9778，精度2，返回4.98
 */
HAPPYCPP_SHARED_LIB_API double Round(const double &d,
                                     const int32_t &precision);

} /* namespace hcdouble */

} /* namespace hcalgorithm */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_DOUBLE_H_
