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

/** @file
 *  VS2015 以 v14.0_xp 编译的程序，运行时执行 stat 函数有 BUG，执行总是失败。
 *  所以，建议使用 boost::filesystem 代替需要 stat 函数的代码。
 */

#ifndef INCLUDE_HAPPYCPP_FILESYS_H_
#define INCLUDE_HAPPYCPP_FILESYS_H_

#include "happycpp/common.h"
#include <fstream>
#include <list>
#include <vector>
#include <string>

namespace happycpp {

    namespace hcfilesys {

        HAPPYCPP_SHARED_LIB_API bool happyCreateFile(const std::string &file);

        // 读取文件，返回字符串
        HAPPYCPP_SHARED_LIB_API std::string readFile(const std::string &file);

        // 读取文件，每行内容为一个元素加入到列表，返回列表，
        HAPPYCPP_SHARED_LIB_API bool readFile(const std::string &file,
                                              std::vector<std::string> *lines);

        HAPPYCPP_SHARED_LIB_API bool writeFile(const std::string &file,
                                               const std::string &content,
                                               const bool &append = false);

        HAPPYCPP_SHARED_LIB_API bool writeFile(const std::string &file,
                                               const std::vector<std::string> &lines,
                                               const bool &append = false);

        // 获取指定目录的文件或子目录列表(不会递归)
        // file_type:
        //     FT_DIR，表示获取目录
        //     FT_FILE，表示获取文件
        //     FT_ALL，表示获取目录和文件
        //
        // max_num:
        //      防止因为超大数量的文件和目录，导致性能问题
        HAPPYCPP_SHARED_LIB_API void getFilesInDir(const std::string &path,
                                                   FileType type,
                                                   std::vector<FileStat> *v,
                                                   uint32_t max_num = 10000);

    } /* namespace hcfilesys */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_FILESYS_H_
