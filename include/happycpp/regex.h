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
 *  使用不支持 C++11 正则表达式的编译器时，用Boost正则表达式库代替
 */

#ifndef INCLUDE_HAPPYCPP_REGEX_H_
#define INCLUDE_HAPPYCPP_REGEX_H_

#if !defined(_USE_CXX_REGEX_) && !defined(_USE_BOOST_REGEX_)
#error Regex macro error.
#endif

// .* 不会匹配多行
// $ 表示行结尾，而非字符串结尾
#ifdef _USE_CXX_REGEX_

#include <regex>

typedef std::regex HappyRegex;
typedef std::smatch HappySmatch;

#define happyRegexCompile(exp) move(std::regex(exp))
// 匹配整个字符串
#define happyRegexMatch(exp1, exp2, exp3) std::regex_match(exp1, exp2, exp3)
// 在字符串中搜索，不需要匹配整个字符串
#define happyRegexSearch(exp1, exp2, exp3) std::regex_search(exp1, exp2, exp3)

#endif

// .* 会匹配多行
// $ 表示字符串结尾
#ifdef _USE_BOOST_REGEX_

#include <boost/xpressive/xpressive.hpp>

namespace bregex = boost::xpressive;

typedef bregex::sregex HappyRegex;
typedef bregex::smatch HappySmatch;

#define happyRegexCompile(exp) move(bregex::sregex::compile(exp))
// 匹配整个字符串
#define happyRegexMatch(exp1, exp2, exp3) bregex::regex_match(exp1, exp2, exp3)
// 在字符串中搜索，不需要匹配整个字符串
#define happyRegexSearch(exp1, exp2, exp3) bregex::regex_search(exp1, exp2, \
                                                                exp3)

#endif

#endif  // INCLUDE_HAPPYCPP_REGEX_H_
