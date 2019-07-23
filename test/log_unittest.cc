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

#include <gtest/gtest.h>
#include <happycpp/filesys.h>
#include <happycpp/log.h>
#include <happycpp/regex.h>
#include <map>

namespace hhlog = happycpp::hclog;
namespace hhfilesys = happycpp::hcfilesys;

void CleanLog() {
  hhfilesys::WriteFile(hhlog::G_LOG_CONFIG.path, "");
}

void CheckLog(const LogLevelNumber level, const std::string &log,
              const std::string &msg, const bool is_regex = false) {
  static std::map<LogLevelNumber, std::string> kLogDESC {
    { LOG_TRACE, "trace" },
    { LOG_DEBUG, "debug" },
    { LOG_INFO, "info" },
    { LOG_WARN, "warn" },
    { LOG_ERROR, "error" },
    { LOG_FATAL, "fatal" },
    { LOG_OFF, "" }
  };

  static HappyRegex re = HappyRegexCompile(
      "^\\[\\d{4}-\\d{2}-\\d{2} \\d{2}:\\d{2}:\\d{2}\\] "  // datetime
      "\\[\\d+\\] "  // pid
      "\\[\\s*(\\w+)\\] "  // log level
      "(.*)" EOL);  // message

  HappySmatch what;

  ASSERT_TRUE(HappyRegexMatch(log, what, re));
  ASSERT_EQ(3U, what.size());
  ASSERT_EQ(kLogDESC[level], std::string(what[1]));

  if (is_regex) {
    // 匹配类似 /path/file.cxx:105 trace
    HappyRegex re2 = HappyRegexCompile(msg);
    HappySmatch what2;
    const std::string msg_from_log(what[2]);
    ASSERT_TRUE(HappyRegexMatch(msg_from_log, what2, re2));
  } else {
    ASSERT_EQ(msg, std::string(what[2]));
  }
}

void test_PrintLog(const LogLevelNumber level, const std::string &msg,
                   const std::string &result, const bool is_regex) {
  CleanLog();
  PrintLog(level, "%s", msg.c_str());
  const std::string log(hhfilesys::ReadFile(hhlog::G_LOG_CONFIG.path));
  CheckLog(level, log, result, is_regex);
}

TEST(HCLOG_UNITTEST, init_test) {
  hhlog::G_LOG_CONFIG.level = LOG_TRACE;
  hhlog::G_LOG_CONFIG.output_to = kStreamFile;
}

TEST(HCLOG_UNITTEST, PrintLog_trace) {
  test_PrintLog(LOG_TRACE, "trace", "(.*):(\\d+) trace", true);
}

TEST(HCLOG_UNITTEST, PrintLog_debug) {
  test_PrintLog(LOG_DEBUG, "debug", "debug", false);
}

TEST(HCLOG_UNITTEST, PrintLog_info) {
  test_PrintLog(LOG_INFO, "info", "info", false);
}

TEST(HCLOG_UNITTEST, PrintLog_warn) {
  test_PrintLog(LOG_WARN, "warn", "warn", false);
}

TEST(HCLOG_UNITTEST, PrintLog_error) {
  test_PrintLog(LOG_ERROR, "error", "error", false);
}

TEST(HCLOG_UNITTEST, PrintLog_fatal) {
  test_PrintLog(LOG_FATAL, "fatal", "fatal", false);
}

TEST(HCLOG_UNITTEST, PrintLog_off) {
  CleanLog();
  PrintLog(LOG_OFF, "off");
  const std::string log(hhfilesys::ReadFile(hhlog::G_LOG_CONFIG.path));
  ASSERT_TRUE(log.empty());
}

TEST(HCLOG_UNITTEST, to_log_level_num) {
  ASSERT_EQ(0, hhlog::to_log_level_num("trace"));
  ASSERT_EQ(1, hhlog::to_log_level_num("debug"));
  ASSERT_EQ(2, hhlog::to_log_level_num("info"));
  ASSERT_EQ(3, hhlog::to_log_level_num("warn"));
  ASSERT_EQ(4, hhlog::to_log_level_num("error"));
  ASSERT_EQ(5, hhlog::to_log_level_num("fatal"));
  ASSERT_EQ(6, hhlog::to_log_level_num("off"));
}

TEST(HCLOG_UNITTEST, to_log_level_str) {
  ASSERT_EQ("trace", hhlog::to_log_level_str(LOG_TRACE));
  ASSERT_EQ("debug", hhlog::to_log_level_str(LOG_DEBUG));
  ASSERT_EQ("info", hhlog::to_log_level_str(LOG_INFO));
  ASSERT_EQ("warn", hhlog::to_log_level_str(LOG_WARN));
  ASSERT_EQ("error", hhlog::to_log_level_str(LOG_ERROR));
  ASSERT_EQ("fatal", hhlog::to_log_level_str(LOG_FATAL));
  ASSERT_EQ("off", hhlog::to_log_level_str(LOG_OFF));
}

TEST(HCLOG_UNITTEST, clean_test) {
  bfs::remove(hhlog::G_LOG_CONFIG.path);
}
