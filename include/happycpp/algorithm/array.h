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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_ARRAY_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_ARRAY_H_

#include "happycpp/common.h"
#include <vector>
#include <list>
#include <algorithm>
#include <sstream>

namespace happycpp {

    namespace hcalgorithm {

        namespace hcarray {

            template<class T>
            inline bool exists(const std::vector<T> &v, const T &e) {
                return (std::find(v.begin(), v.end(), e) != v.end());
            }

            // 检查数组中是否存在指定元素
            template<class T>
            inline bool find(std::vector<T> *v, const T &e,
                             typename std::vector<T>::iterator *it) {
                typename std::vector<T>::iterator _it = std::find(v->begin(), v->end(), e);

                if (_it == v->end())
                    return false;

                *it = _it;
                return true;
            }

            // 删除数组中所有重复的元素
            template<class T>
            inline void unique(std::vector<T> *v) {
                if (v->empty())
                    return;

                // 去重之前必须排序
                std::sort(v->begin(), v->end());

                // 获取重复元素位置
                typename std::vector<T>::iterator it = std::unique(v->begin(), v->end());

                if (it != v->end())
                    v->erase(it, v->end());
            }

            // 将数组转为字符串，可带分隔符
            template<class T>
            inline std::string toString(std::vector<T> &v, const std::string &delimiter = "") {
                std::stringstream ss;

                const size_t length = v.size();

                for (size_t i = 0; i < length; i++) {
                    ss << v[i];

                    if (i != length - 1) {
                        ss << delimiter;
                    }
                }

                return ss.str();
            }

        } /* namespace hcarray */

    } /* namespace hcalgorithm */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_ARRAY_H_
