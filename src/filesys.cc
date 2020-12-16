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

#include <happycpp/filesys.h>
#include <happycpp/log.h>
#include <happycpp/hcerrno.h>
#include <happycpp/exception.h>
#include <happycpp/algorithm/hcstring.h>

#ifdef PLATFORM_WIN32
#include <direct.h>
#ifndef INCLUDE_WINDOWS_H_FILE
#define INCLUDE_WINDOWS_H_FILE
#include <Windows.h>
#endif
#else

#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>

#endif

#include <sys/stat.h>

#ifdef PLATFORM_WIN32
// Windows:
// 1. 目录链接识别为 目录
// 2. 文件链接识别为 文件
#define S_ISDIR(st_mode) (((st_mode) & S_IFMT) == S_IFDIR)
#define S_ISLNK(st_mode) 0
#define S_ISREG(st_mode) (((st_mode) & S_IFMT) == S_IFREG)
#endif

using std::ifstream;
using std::ofstream;
using happycpp::hcerrno::errorToStr;
using happycpp::hcalgorithm::hcstring::toLower;

namespace happycpp::hcfilesys {

        HAPPYCPP_SHARED_LIB_API bool happyCreateFile(const std::string &file) {
            if (bfs::exists(file))
                return true;

#ifdef PLATFORM_WIN32
            HANDLE h = CreateFile(file.c_str(),  // file name
                                  GENERIC_WRITE,  // open for write
                                  0,  // do not share
                                  NULL,  // default security
                                  CREATE_ALWAYS,  // overwrite existing
                                  FILE_ATTRIBUTE_NORMAL,  // normal file
                                  NULL);  // no template

            if (h) {
              CloseHandle(h);
              return true;
            }
#else
            mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
            int fd = creat(file.c_str(), mode);

            if (fd) {
                close(fd);
                return true;
            }
#endif

            return false;
        }

        HAPPYCPP_SHARED_LIB_API std::string readFile(const std::string &file) {
            ifstream ifs(file.c_str(), ifstream::binary);

            if (!ifs)
                ThrowHappyException(errorToStr());

            std::stringstream buffer;
            buffer << ifs.rdbuf();

            ifs.close();

            return buffer.str();
        }

        HAPPYCPP_SHARED_LIB_API bool readFile(const std::string &file,
                                              std::vector<std::string> *lines) {
            lines->clear();
            std::string tmp;
            std::string content;
            ifstream ifs(file.c_str(), ifstream::binary);

            if (!ifs) {
                happycpp::log::HappyLogPtr hlog = happycpp::log::HappyLog::getInstance();
                hlog->error(errorToStr());
                return false;
            }

            // 以 '\n' 作为分行标识
            while (getline(ifs, tmp)) {
                const size_t tmp_size = tmp.size();

                if (tmp_size && tmp[tmp_size - 1] == '\r')
                    tmp.erase(tmp_size - 1);

                lines->push_back(tmp);
                tmp.clear();
            }

            ifs.close();
            return true;
        }

        HAPPYCPP_SHARED_LIB_API bool writeFile(const std::string &file,
                                               const std::vector<std::string> &lines,
                                               const bool &append) {
            std::string content;

            for (const auto &x : lines)
                content += x + EOL;

            return writeFile(file, content, append);
        }

        HAPPYCPP_SHARED_LIB_API bool writeFile(const std::string &file,
                                               const std::string &content,
                                               const bool &append) {
            ofstream ofs;

            if (append)
                ofs.open(file.c_str(), ofstream::binary | ofstream::ate | ofstream::app);
            else
                ofs.open(file.c_str(), ofstream::binary);

            if (ofs) {
                ofs << content;
            } else {
                happycpp::log::HappyLogPtr hlog = happycpp::log::HappyLog::getInstance();
                hlog->error(errorToStr());
                return false;
            }

            return true;
        }

        HAPPYCPP_SHARED_LIB_API void getFilesInDir(const std::string &path,
                                                   FileType type,
                                                   std::vector<FileStat> *v,
                                                   uint32_t max_num) {
            if (path.empty())
                ThrowHappyException("Invalid directory");

            v->clear();

            uint32_t num = 0;
            FileType _type;
            std::string ext;
            std::string _path;
            std::string name;
            size_t sep_pos;
            FileStat file_stat;
            struct stat _stat{};
#ifdef PLATFORM_WIN32
            WIN32_FIND_DATA wfd;
            HANDLE handle = FindFirstFile(std::string(path + "\\*.*").c_str(), &wfd);

            if (handle == INVALID_HANDLE_VALUE)
              ThrowHappyException(hcerrno::ErrorToStr());

            do {
              name = wfd.cFileName;
#else
            DIR *dir;
            struct dirent *d;

            dir = opendir(path.c_str());

            if (dir == nullptr)
                ThrowHappyException(hcerrno::errorToStr());

            // 遍历目录，找出目录和文件
            while ((d = readdir(dir))) {
                name = d->d_name;
#endif
                // 跳过 . 和 ..
                if (name == "." || name == "..")
                    continue;

                _path = "";
                _path.append(path);
                _path.append(OsSeparator);
                _path.append(name);

                // 检测能否获取文件信息
                if (stat(_path.c_str(), &_stat) != 0)
                    ThrowHappyException(hcerrno::errorToStr());

                if (S_ISDIR(_stat.st_mode)) {
                    _type = kDir;
                } else if (S_ISREG(_stat.st_mode) || S_ISLNK(_stat.st_mode)) {
                    _type = kFile;
                    // 查找字符串中最后一个点，
                    // 如果有则截取点到字符串末尾之间的子字符串作为扩展名
                    sep_pos = name.find_last_of('.');

                    if (sep_pos != std::string::npos)
                        ext = toLower(name.substr(sep_pos));
                } else {
                    continue;
                }

                if (type == kAll || type == _type) {
                    file_stat.type = _type;
                    file_stat.name = name;
                    file_stat.ext = ext;
                    file_stat.path = _path;
                    file_stat.bytes = _stat.st_size;
                    file_stat.atime = _stat.st_atime;
                    file_stat.ctime = _stat.st_ctime;
                    file_stat.mtime = _stat.st_mtime;
                    v->push_back(file_stat);
                    ++num;

                    if (num == max_num) {
                        happycpp::log::HappyLogPtr hlog = happycpp::log::HappyLog::getInstance();
                        hlog->error("Too many files or directorys in \"" + _path + "\".");
                        break;
                    }
                }
#ifdef PLATFORM_WIN32
                } while (FindNextFile(handle, &wfd));
#else
            }
#endif

#ifdef PLATFORM_WIN32
            FindClose(handle);
#else
            closedir(dir);
#endif
    }

} /* namespace happycpp */
