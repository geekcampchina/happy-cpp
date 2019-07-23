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

#ifndef INCLUDE_HAPPYCPP_TYPE_H_
#define INCLUDE_HAPPYCPP_TYPE_H_

#include <stdint.h>
#include <string>

// for size_t, ssize_t
#ifdef PLATFORM_WIN32
#if !defined(_SSIZE_T_) && !defined(_SSIZE_T_DEFINED)
typedef intptr_t ssize_t;
#define _SSIZE_T_
#define _SSIZE_T_DEFINED
#endif
#else
#include <sys/types.h>
#endif

typedef uint64_t byte_t;
typedef double kib_t;
typedef double mib_t;
typedef double gib_t;
typedef double tib_t;
typedef double pib_t;
typedef double kb_t;
typedef double mb_t;
typedef double gb_t;
typedef double tb_t;
typedef double pb_t;

// 文件类型
typedef enum {
  kFile,
  kDir,
  kAll
} FileType;

// 比较结果
typedef enum {
  kLess = -1,
  kEqual,  // 0
  kGreater  // 1
} CmpResult;

typedef enum {
  kStreamFile,
  kStreamStdout,
  kStreamStderr
} LogOutputStream;

typedef enum {
  LOG_TRACE,
  LOG_DEBUG,
  LOG_INFO,
  LOG_WARN,
  LOG_ERROR,
  LOG_FATAL,
  LOG_OFF
} LogLevelNumber;

typedef struct {
  uint32_t level;
  LogOutputStream output_to;
  size_t max_byte;  // 0 为不限制大小
  std::string path;
  std::string default_dir;  // 默认日志目录
} LogConfig;

/*
 ----------字节的次方单位----------
 十进制前缀(SI)标准：
 名字  缩写 次方
 kilobyte  KB  10的3次方
 megabyte  MB  10的6次方
 gigabyte  GB  10的9次方
 terabyte  TB  10的12次方
 petabyte  PB  10的15次方
 exabyte   EB  10的18次方
 zettabyte   ZB  10的21次方
 yottabyte   YB  10的24次方
 ------------------------------
 二进制前缀(IEC 60027-2)标准：
 名字      缩写  次方
 kibibyte  KiB   2的10次方
 mebibyte  MiB   2的20次方
 gibibyte  GiB   2的30次方
 tebibyte  TiB   2的40次方
 pebibyte  PiB   2的50次方
 exbibyte  EiB   2的60次方
 zebibyte  ZiB   2的70次方
 yobibyte  YiB   2的80次方
 */
// 二进制乘数词头枚举，不支持EiB、ZiB和YiB
// 国际单位制词头枚举，不支持EB、ZB和YB
typedef enum {
  kUnitByte = 0,  // byte
  // 二进制乘数词头 {
  kUnitKiB,  // kibibyte,
  kUnitMiB,  // mebibyte,
  kUnitGiB,  // gibibyte,
  kUnitTiB,  // tebibyte,
  kUnitPiB,  // pebibyte,
  // }
  // 国际单位制词头 {
  kUnitKB,  // kilobyte,
  kUnitMB,  // megabyte,
  kUnitGB,  // gigabyte,
  kUnitTB,  // terabyte,
  kUnitPB,  // petabyte,
  // }
} UnitType;

typedef enum {
  kAlnum,  // 字母以及数字
  kAlpha,  // 字母
  kDigit,  // 数字
  kPrint  // 可见字符
} CharClassification;

// 文件或者目录状态
typedef struct {
  FileType type;  // 文件类型： 文件、目录 或 所有(文件和目录)
  std::string name;  // 文件名
  std::string ext;  // 文件扩展名，小写
  std::string path;  // 文件全路径
  byte_t bytes;  // 文件大小，单位字节(byte)。如果是目录，则为0
  time_t atime;  // 文件访问时间，秒
  time_t ctime;  // 文件状态修改时间，秒
  time_t mtime;  // 文件修改时间，秒
} FileStat;

#endif  // INCLUDE_HAPPYCPP_TYPE_H_
