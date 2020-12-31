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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_MAP_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_MAP_H_

#include <boost/bind/bind.hpp>
#include <algorithm>
#include <map>

using namespace boost::placeholders;

namespace happycpp::hcalgorithm::hcmap {

            template<class T, class V>
            typename T::const_iterator findByValue(const T &m, const V &value) {
                typename T::const_iterator it = std::find_if(
                        m.begin(), m.end(), boost::bind(&T::value_type::second, _1) == value);

                return it;
            }

            template<class T1, class T2>
            void swapKeyValue(const T1 &m1, T2 *m2) {
                for (typename T1::const_iterator it = m1.begin(); it != m1.end(); ++it)
                    (*m2)[it->second] = it->first;
            }

        } /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_MAP_H_
