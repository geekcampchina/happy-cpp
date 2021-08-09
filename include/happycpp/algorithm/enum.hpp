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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_HAPPY_CPP_HENUM_HPP_
#define INCLUDE_HAPPYCPP_ALGORITHM_HAPPY_CPP_HENUM_HPP_

#include <string>
#include <map>
#include "happycpp/common.h"
#include "happycpp/exception.h"

#define DEFINE_ENUM_MAP_ITEM(ENUM) {ENUM, #ENUM}

template <typename T>
using enum_map = std::map<T, std::string>;

namespace happycpp::hcalgorithm::henum {
    // 由于magic_enum无法在enum数量超过255后，编译需要巨量内存。
    // EnumPlus（数据查询表）作为替代方案
    template<typename T, typename Y>
    class EnumPlus {
    private:
        std::map<T, std::string> *m_ = nullptr;

    public:
        explicit EnumPlus(std::map<T, std::string> &m) {
            m_ = &m;
        }

        T toEnum(Y value) {
            if (exists(value)) {
                return static_cast<T>(value);
            }

            ThrowHappyException("无效的枚举值");
        }

        bool exists(Y value) {
            const T t = static_cast<T>(value);
            return m_->find(t) != m_->end();
        }

        std::string toName(T value) {
            if (!exists(value)) {
                return "";
            }

            const auto it = m_->find(value);

            if (it == m_->end()) {
                return "";
            }

            return it->second;
        }

        std::string toName(Y value) {
            return toName(toEnum(value));
        }
    };
} /* namespace happycpp */

#endif //INCLUDE_HAPPYCPP_ALGORITHM_HAPPY_CPP_HENUM_HPP_
