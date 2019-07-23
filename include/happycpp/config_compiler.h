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

// 会被多个文件同时包含，不需要定义头文件宏
// #ifndef INCLUDE_HAPPYCPP_CONFIG_COMPILER_H_
// #define INCLUDE_HAPPYCPP_CONFIG_COMPILER_H_

#ifdef HAPPYCPP_STATICLIB
#define HAPPYCPP_SHARED_LIB_API
#endif

#ifdef _MSC_VER
# if !defined(HAPPYCPP_STATICLIB) && !defined(HAPPYCPP_SHAREDLIB)
#   error No found macro HAPPYCPP_STATICLIB or HAPPYCPP_SHAREDLIB.
# endif

# if !defined(HAPPYCPP_SHARED_LIB_API)
#   if defined(HAPPYCPP_SHAREDLIB)
// If we are building the DLL, export the symbols tagged like this
#     define HAPPYCPP_SHARED_LIB_API __declspec(dllexport)
#   else
// If we are consuming the DLL, import the symbols tagged like this
#     define HAPPYCPP_SHARED_LIB_API __declspec(dllimport)
#   endif
# endif
#elif defined(__GNUC__)
# if !defined(HAPPYCPP_SHARED_LIB_API)
#   if defined(HAPPYCPP_SHAREDLIB)
#     define HAPPYCPP_SHARED_LIB_API __attribute__ ((visibility (\
"default")))
#   else
// If you use -fvisibility=hidden in GCC, exception handling and RTTI
// would break if visibility wasn't set during export _and_ import
// because GCC would immediately forget all type infos encountered.
// See http://gcc.gnu.org/wiki/Visibility
#   define HAPPYCPP_SHARED_LIB_API __attribute__ ((visibility (\
"default")))
#   endif
# endif
#else
# error Unknown compiler
#endif

// #endif  // INCLUDE_HAPPYCPP_CONFIG_COMPILER_H_
