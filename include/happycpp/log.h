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

#ifndef INCLUDE_HAPPYCPP_LOG_H_
#define INCLUDE_HAPPYCPP_LOG_H_

#include "happycpp/common.h"
#include "log4cplus/logger.h"
#include "log4cplus/loggingmacros.h"
#include "log4cplus/loglevel.h"
#include <boost/filesystem.hpp>

#define DEFAULT_LOG_PROFILE_NAME "log4cplus.properties"

namespace happycpp::log {
    /*
     * 示例代码：
    #include "happycpp/happylog.h"

    using namespace happycpp::log;

    fs::path p = fs::current_path() / DEFAULT_LOG_PROFILE_NAME;
    HappyLogPtr hlog = HappyLog::getInstance(p);
    hlog->info("test message......");
     */
    class HappyLog {
    private:
        log4cplus::Logger _logger;
        static std::shared_ptr<HappyLog> _instance;
        std::string _logPrefix;

    public:
        static std::shared_ptr<HappyLog>
        getInstance(log4cplus::LogLevel level = log4cplus::INFO_LOG_LEVEL, const std::string &logPrefix = "");

        static std::shared_ptr<HappyLog> getInstance(const std::string &profile, const std::string &logPrefix = "");

        static std::shared_ptr<HappyLog>
        getInstance(const boost::filesystem::path &profile, const std::string &logPrefix = "");

        void enterFunc(const std::string &funcName, const std::string &loggerName = "root");

        void exitFunc(const std::string &funcName, const std::string &loggerName = "root");

        // 泛型被其它动态库调用时，会出现"Undefined symbols error"错误，将函数实现放在头文件中即可
        template<typename T>
        void var(const std::string &name, T value, const std::string &loggerName = "root") {
            LOG4CPLUS_TRACE(_logger, LOG4CPLUS_TEXT(_logPrefix + "var->") << name << LOG4CPLUS_TEXT("=") << value);
        }

        // 泛型被其它动态库调用时，会出现"Undefined symbols error"错误，将函数实现放在头文件中即可
        template<typename T>
        void input(const std::string &name, T value, const std::string &loggerName = "root") {
            LOG4CPLUS_TRACE(_logger, LOG4CPLUS_TEXT(_logPrefix + "input->") << name << LOG4CPLUS_TEXT("=") << value);
        }

        // 泛型被其它动态库调用时，会出现"Undefined symbols error"错误，将函数实现放在头文件中即可
        template<typename T>
        void output(const std::string &name, T value, const std::string &loggerName = "root") {
            LOG4CPLUS_TRACE(_logger, LOG4CPLUS_TEXT(_logPrefix + "output->") << name << LOG4CPLUS_TEXT("=") << value);
        }

        void error(const std::string &s, const std::string &loggerName = "root");

        void error(const std::exception &e, const std::string &loggerName = "root");

        void warn(const std::string &s, const std::string &loggerName = "root");

        void info(const std::string &s, const std::string &loggerName = "root");

        void debug(const std::string &s, const std::string &loggerName = "root");

        void trace(const std::string &s, const std::string &loggerName = "root");

        void setLogPrefix(const std::string &logPrefix);

    protected:
        explicit HappyLog(const std::string &profile, const std::string &logPrefix = "");

        explicit HappyLog(log4cplus::LogLevel level = log4cplus::INFO_LOG_LEVEL, const std::string &logPrefix = "");
    };

    typedef std::shared_ptr<HappyLog> HappyLogPtr;
} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_LOG_H_