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

#include "happycpp/algorithm/unit.h"
#include "happycpp/algorithm/double.h"
#include <map>

using std::map;

namespace happycpp {

    namespace hcalgorithm {

        namespace hcunit {

            HAPPYCPP_SHARED_LIB_API std::string toStr(UnitType ut) {
                static std::map<UnitType, std::string> m;
                m[kUnitByte] = "byte";
                m[kUnitKiB] = "KiB";
                m[kUnitMiB] = "MiB";
                m[kUnitGiB] = "GiB";
                m[kUnitTiB] = "TiB";
                m[kUnitPiB] = "PiB";
                m[kUnitKB] = "KB";
                m[kUnitMB] = "MB";
                m[kUnitGB] = "GB";
                m[kUnitTB] = "TB";
                m[kUnitPB] = "PB";

                return m[ut];
            }

            HAPPYCPP_SHARED_LIB_API byteSize_t toByte(UnitType ut) {
                static std::map<UnitType, byteSize_t> m;
                m[kUnitByte] = 1L;
                m[kUnitKiB] = 1024L;
                m[kUnitMiB] = 1048576L;
                m[kUnitGiB] = 1073741824L;
                m[kUnitTiB] = 1099511627776L;
                m[kUnitPiB] = 1125899906842624L;
                m[kUnitKB] = 1000L;
                m[kUnitMB] = 1000000L;
                m[kUnitGB] = 1000000000L;
                m[kUnitTB] = 1000000000000L;
                m[kUnitPB] = 1000000000000000L;

                return m[ut];
            }

            HAPPYCPP_SHARED_LIB_API byteSize_t toByte(UnitType ut, double s) {
                return toByte(ut) * s;
            }

            HAPPYCPP_SHARED_LIB_API double convert(UnitType ut, byteSize_t b,
                                                   uint32_t precision) {
                if (ut == kUnitByte)
                    return static_cast<double>(b);

                double size = static_cast<double>(b) / toByte(ut);

                if (precision == 0)
                    // 向上取整
                    size = ceil(size);
                else
                    // 四舍五入，并保留指定的有效数
                    size = hcdouble::round(size, precision);

                return size;
            }

        } /* namespace hcunit */

    } /* namespace hcalgorithm */

} /* namespace happycpp */
