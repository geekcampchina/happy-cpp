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

#ifndef INCLUDE_HAPPY_LOG_H_
#define INCLUDE_HAPPY_LOG_H_

#include <happycpp/common.h>
#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"
#include <boost/filesystem.hpp>

#define DEFAULT_LOG_PROFILE_NAME "log4cplus.properties"

namespace fs = boost::filesystem;

namespace happycpp {
    namespace log {
        class HappyLog {
        private:
            log4cplus::Logger _logger;
            static std::shared_ptr<HappyLog> _instance;

        public:
            static std::shared_ptr<HappyLog> getInstance(const std::string &profile);
            static std::shared_ptr<HappyLog> getInstance(const boost::filesystem::path& profile);

            void enterFunc(const std::string &funcName);

            void exitFunc(const std::string &funcName);

            // 泛型被其它动态库调用时，会出现"Undefined symbols error"错误，将函数实现放在头文件中即可
            template<typename T>
            void var(const std::string &name, T value) {
                LOG4CPLUS_TRACE(_logger, LOG4CPLUS_TEXT("var->") << name << LOG4CPLUS_TEXT("=") << value);
            }

            // 泛型被其它动态库调用时，会出现"Undefined symbols error"错误，将函数实现放在头文件中即可
            template<typename T>
            void input(const std::string &name, T value) {
                LOG4CPLUS_TRACE(_logger, LOG4CPLUS_TEXT("input->") << name << LOG4CPLUS_TEXT("=") << value);
            }

            // 泛型被其它动态库调用时，会出现"Undefined symbols error"错误，将函数实现放在头文件中即可
            template<typename T>
            void output(const std::string &name, T value) {
                LOG4CPLUS_TRACE(_logger, LOG4CPLUS_TEXT("output->") << name << LOG4CPLUS_TEXT("=") << value);
            }

            void error(const std::string &s);

            void warn(const std::string &s);

            void info(const std::string &s);

            void debug(const std::string &s);

            void trace(const std::string &s);

        protected:
            explicit HappyLog(const std::string &profile);
        };

        typedef std::shared_ptr<HappyLog> HappyLogPtr;
    } /* namespace log */
} /* namespace happycpp */

#endif  // INCLUDE_HAPPY_LOG_H_
