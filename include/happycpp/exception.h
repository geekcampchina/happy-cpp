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

#ifndef INCLUDE_HAPPYCPP_EXCEPTION_H_
#define INCLUDE_HAPPYCPP_EXCEPTION_H_

#include "happycpp/common.h"
#include "happycpp/log.h"
#include <exception>
#include <stdexcept>
#include <string>

namespace happycpp {

    class HAPPYCPP_SHARED_LIB_API HappyException : public std::exception {
    private:
        happycpp::log::HappyLogPtr hlog = happycpp::log::HappyLog::getInstance();
        std::runtime_error _error;

    public:
        // explicit 只对构造函数起作用，用来抑制隐式转换。
        explicit HappyException(const std::string &msg);

        ~HappyException() noexcept override;

        [[nodiscard]] const char *what() const noexcept override;
    };

} /* namespace happycpp */

#define ThrowHappyException(msg) \
  throw happycpp::HappyException(msg)

#endif  // INCLUDE_HAPPYCPP_EXCEPTION_H_
