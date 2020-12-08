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

#include "happycpp/algorithm/random.h"
#include <cctype>
#include <random>

namespace happycpp {

    namespace hcalgorithm {

        namespace hcrandom {
            HAPPYCPP_SHARED_LIB_API uint64_t getRandom() {
                std::random_device rd;
                return rd();
            }

            HAPPYCPP_SHARED_LIB_API std::string genRandom(const size_t &size,
                                                          CharClassification cc) {
                std::string str;
                size_t len = 0;

                do {
                    int c = getRandom() % 126;
                    bool ret_val = false;

                    switch (cc) {
                        case kAlnum:
                            ret_val = std::isalnum(c);
                            break;
                        case kAlpha:
                            ret_val = std::isalpha(c);
                            break;
                        case kDigit:
                            ret_val = std::isdigit(c);
                            break;
                        case kPrint:
                            ret_val = (!std::isspace(c) && std::isprint(c));
                            break;
                        default:
                            break;
                    }

                    if (ret_val) {
                        str.push_back(c);
                        ++len;
                    }
                } while (len < size);

                return str;
            }

        } /* namespace hcrandom */

    } /* namespace hcalgorithm */

} /* namespace happycpp */
