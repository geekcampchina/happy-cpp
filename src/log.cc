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

#include <happycpp/log.h>
#include <happycpp/common.h>
#include <happycpp/config_platform.h>
#include <happycpp/errno.h>
#include <map>

#ifdef PLATFORM_WIN32
#include <process.h>

#ifndef INCLUDE_WINDOWS_H_FILE
#define INCLUDE_WINDOWS_H_FILE
#include <Windows.h>
#endif
#else
#include <unistd.h>
#endif

#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#include <cstring>
#include <ctime>

using std::map;
using happycpp::hcerrno::ErrorToStr;

namespace happycpp {

namespace hclog {

static std::map<LogLevelNumber, std::string> kLogDESC {
  { LOG_TRACE, "[trace]" },
  { LOG_DEBUG, "[debug]" },
  { LOG_INFO, "[ info]" },
  { LOG_WARN, "[ warn]" },
  { LOG_ERROR, "[error]" },
  { LOG_FATAL, "[fatal]" },
  { LOG_OFF, "" }
};

#ifdef PLATFORM_WIN32
// 104857600 byte == 100 MiB
LogConfig G_LOG_CONFIG {
    LOG_INFO, kStreamFile, 104857600,
    "C:\\Windows\\temp\\tmp.log",
    "C:\\Windows\\temp"
};
#else
// 104857600 byte == 100 MiB
LogConfig G_LOG_CONFIG {
  LOG_INFO, kStreamFile, 104857600, "/tmp/tmp.log", "/tmp"
};
#endif

byte_t FileSize(const char *file) {
  FILE *f = fopen(file, "r");

  if (!f)
    return 0;

  HAPPY_ASSERT(fseek(f, 0, SEEK_END) == 0);
  const int64_t size = ftell(f);
  HAPPY_ASSERT(size >= 0);
  HAPPY_ASSERT(fseek(f, 0, SEEK_SET) == 0);

  fclose(f);
  return static_cast<byte_t>(size);
}

void OpenLogFile(FILE **file) {
  // vs2015 v14.0_xp 编译的运行时，stat 函数有BUG
  const size_t old_log_size = FileSize(G_LOG_CONFIG.path.c_str());

  if (G_LOG_CONFIG.max_byte == 0)
    // 追加
    *file = fopen(G_LOG_CONFIG.path.c_str(), "ab+");
  else if (old_log_size >= G_LOG_CONFIG.max_byte)
    // 覆盖
    *file = fopen(G_LOG_CONFIG.path.c_str(), "wb");
  else
    // 追加
    *file = fopen(G_LOG_CONFIG.path.c_str(), "ab+");

  if (*file == NULL) {
    fprintf(stderr,
            "cannot open \"%s\": %s" EOL,
            G_LOG_CONFIG.path.c_str(),
            ErrorToStr().c_str());

    exit(EXIT_FAILURE);
  }
}

HAPPYCPP_SHARED_LIB_API std::string GetCurrentDate() {
  char buf[80];
  time_t t = time(NULL);
  strftime(buf, 80, "%Y-%m-%d %H:%M:%S", localtime(&t));
  return std::string(buf);
}

HAPPYCPP_SHARED_LIB_API void _PrintLog(LogLevelNumber level, const char *file,
                                       int line, const char *fmt, ...) {
  // 关闭日志
  if (level == LOG_OFF)
    return;

  // 关闭比用户设置更加详细的日志
  if (level < G_LOG_CONFIG.level)
    return;

  FILE *log_file = NULL;

  switch (G_LOG_CONFIG.output_to) {
    case kStreamStdout:
      log_file = stdout;
      break;
    case kStreamStderr:
      log_file = stderr;
      break;
    case kStreamFile:
      OpenLogFile(&log_file);
      break;
    default:
      HAPPY_ASSERT(false);
  }

#ifdef PLATFORM_WIN32
  static int pid = _getpid();
#else
  static int pid = getpid();
#endif

  fprintf(log_file,
          "[%s] [%d] %s ",
          GetCurrentDate().c_str(),
          pid,
          kLogDESC[level].c_str());

  if (level <= LOG_TRACE)
    fprintf(log_file, "%s:%d ", file, line);

  va_list ap;
  va_start(ap, fmt);
  vfprintf(log_file, fmt, ap);
  va_end(ap);

  fprintf(log_file, EOL);
  fflush(log_file);

  if (G_LOG_CONFIG.output_to == kStreamFile)
    fclose(log_file);
}

int to_log_level_num(const std::string &level_str) {
  std::map<std::string, int> desc_loglevel_map {
    {"trace", LOG_TRACE},
    {"debug", LOG_DEBUG},
    {"info", LOG_INFO},
    {"warn", LOG_WARN},
    {"error", LOG_ERROR},
    {"fatal", LOG_FATAL},
    {"off", LOG_OFF}
  };

  std::map<std::string, int>::const_iterator it =
      desc_loglevel_map.find(level_str);
  HAPPY_ASSERT(it != desc_loglevel_map.end());

  return it->second;
}

std::string to_log_level_str(const int level_num) {
  std::map<int, std::string> loglevel_desc_map {
    {LOG_TRACE, "trace"},
    {LOG_DEBUG, "debug"},
    {LOG_INFO, "info"},
    {LOG_WARN, "warn"},
    {LOG_ERROR, "error"},
    {LOG_FATAL, "fatal"},
    {LOG_OFF, "off"}
  };

  std::map<int, std::string>::const_iterator it =
      loglevel_desc_map.find(level_num);
  HAPPY_ASSERT(it != loglevel_desc_map.end());

  return it->second;
}

} /* namespace hclog */

} /* namespace happycpp */
