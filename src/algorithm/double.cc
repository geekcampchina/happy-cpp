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

#include <happycpp/algorithm/double.h>
#include <cmath>
#include <sstream>

using std::stringstream;

namespace happycpp {

namespace hcalgorithm {

namespace hcdouble {

HAPPYCPP_SHARED_LIB_API double Round(const double &d,
                                     const int32_t &precision) {
  double i = 0.0;
  double y = 0.0;

  if (d == 0)
    return d;

  i = d * pow(10.00, static_cast<double>(precision));

  if (d > 0)
    i = static_cast<int64_t>(i + 0.5);
  else if (d < 0)
    i = static_cast<int64_t>(i - 0.5);

  y = i * pow(10.00, -precision);

  return y;
}

} /* namespace hcdouble */

} /* namespace hcalgorithm */

} /* namespace happycpp */
