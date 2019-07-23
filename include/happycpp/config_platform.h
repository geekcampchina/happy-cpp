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

#ifndef INCLUDE_HAPPYCPP_CONFIG_PLATFORM_H_
#define INCLUDE_HAPPYCPP_CONFIG_PLATFORM_H_

#if defined(WIN32) || defined(_WIN32)
#define PLATFORM_WIN32 1
#elif defined(__linux__) || defined(linux)
#define PLATFORM_LINUX 1
#else
#error Unknown platform
#endif

#if defined(_WIN32) || defined(_WIN64)
#  if defined(_WIN64)
#    define PLATFORM_64 1
#  else
#    define PLATFORM_32 1
#  endif
#elif defined(__GNUC__)
#  if defined(__x86_64__)
#    define PLATFORM_64 1
#  else
#    define PLATFORM_32 1
#  endif
#else
#  error Unknown platform
#endif

#endif  // INCLUDE_HAPPYCPP_CONFIG_PLATFORM_H_
