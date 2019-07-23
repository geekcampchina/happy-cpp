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

#include <happycpp/proc.h>
#include <happycpp/filesys.h>
#include <happycpp/exception.h>
#ifdef PLATFORM_WIN32
#include <comdef.h>
#else
#include <errno.h>
#include <linux/limits.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#endif
#include <cstdlib>

using std::to_string;

namespace happycpp {

namespace hcproc {

HAPPYCPP_SHARED_LIB_API bool CurrentWorkDir(std::string *dir) {
  dir->clear();

#ifdef PLATFORM_WIN32
  TCHAR t_buf[MAX_PATH];
  LPTSTR lpt_str = t_buf;

  if (GetCurrentDirectory(MAX_PATH, lpt_str))
    *dir = move(std::string(lpt_str));
#else
  char *buffer = get_current_dir_name();

  if (buffer != NULL) {
    *dir = move(std::string(buffer));
    free(buffer);
  }
#endif
  return (dir->size() != 0);
}

HAPPYCPP_SHARED_LIB_API bool CurrentExePath(std::string *path) {
  path->clear();
#ifdef PLATFORM_WIN32
  TCHAR sz_path[MAX_PATH];

  if (GetModuleFileName(NULL, sz_path, MAX_PATH))
    *path = move(std::string(sz_path));
#else
  char buffer[PATH_MAX];
  ssize_t size = readlink("/proc/self/exe", buffer, PATH_MAX - 1);

  if (size != -1) {
    buffer[size] = '\0';
    *path = move(std::string(buffer));
  }
#endif
  return (path->size() != 0);
}

HAPPYCPP_SHARED_LIB_API bool CurrentExeDir(std::string *dir) {
  if (CurrentExePath(dir)) {
    *dir = bfs::path(*dir).parent_path().string();
    return true;
  }

  return false;
}

#ifdef PLATFORM_WIN32
HAPPYCPP_SHARED_LIB_API bool LockProc(const std::string &proc) {
  HANDLE hMutex = CreateMutex(NULL, false, proc.c_str());

  return !(GetLastError() == ERROR_ALREADY_EXISTS);
}

HAPPYCPP_SHARED_LIB_API bool UnLockProc(const std::string &proc) {
  HANDLE hMutex = OpenMutex(MUTEX_ALL_ACCESS, false, proc.c_str());

  if (hMutex == NULL) return false;

  CloseHandle(hMutex);
  return true;
}
#else
HAPPYCPP_SHARED_LIB_API bool LockProc(const std::string &proc, const pid_t pid,
                                      const std::string &prefix) {
  const std::string pid_file(prefix + "/" + proc + ".pid");

  // 进程存在
  if (bfs::exists(pid_file)) {
    const std::string pid(hcfilesys::ReadFile(pid_file));

    if (bfs::exists("/proc/" + pid))
      return false;
  }

  try {
    hcfilesys::WriteFile(pid_file, to_string(pid));
  } catch (HappyException &e) {
    return false;
  }

  return true;
}

HAPPYCPP_SHARED_LIB_API bool UnLockProc(const std::string &proc,
                                        const std::string &prefix) {
  const std::string pid_file(prefix + "/" + proc + ".pid");

  if (bfs::exists(pid_file))
    return bfs::remove(pid_file);

  return false;
}

HAPPYCPP_SHARED_LIB_API void CreateDaemon(const std::string &name,
                                          const bool to_null) {
  if (name.empty())
    ThrowHappyException("Daemon name is empty.");

  if (!hcproc::LockProc(name, getpid()))
    ThrowHappyException(
        "Another instance of " + name + " is already running");

  const int noclose(to_null ? 0 : 1);

  if (daemon(0, noclose) != 0) {
    UnLockProc(name);
    ThrowHappyException(
        "Cannot run " + name + " in the background as system daemons.");
  }
}
#endif

} /* namespace hcproc */

} /* namespace happycpp */
