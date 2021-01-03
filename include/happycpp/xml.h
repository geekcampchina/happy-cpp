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

#ifndef INCLUDE_HAPPYCPP_XML_H_
#define INCLUDE_HAPPYCPP_XML_H_

#include "happycpp/algorithm.h"
#include "happycpp/exception.h"
#include <pugixml.hpp>
#include <string>

using happycpp::hcalgorithm::hcstring::isAlpha;
using happycpp::hcalgorithm::hcstring::isDigit;
using happycpp::hcalgorithm::hcstring::toLower;

namespace happycpp::hcxml {

    typedef enum {
        VM_STRICT,  // 严格
        VM_LOOSE  // 自由的
    } value_mode_t;

    /* 在 XML 字符串中查找指定的节点，返回匹配的节点数量
     * xpath表示指定的节点路径 */
    HAPPYCPP_SHARED_LIB_API size_t specNodeSize(const std::string &xml,
                                                const std::string &xpath);

    HAPPYCPP_SHARED_LIB_API std::string loadFromFile(const std::string &f);

    // 固定的文件头
    // <?xml version="1.0" encoding="UTF-8"?>
    HAPPYCPP_SHARED_LIB_API std::string toStr(pugi::xml_document *doc,
                                              const std::string &indent = "\t");

    /* 格式化 XML 字符串*/
    HAPPYCPP_SHARED_LIB_API std::string format(const std::string &xml,
                                               const std::string &indent = "\t");

    /* 验证字符串是否是xml格式 */
    HAPPYCPP_SHARED_LIB_API bool validate(const std::string &xml);

    /* 获取xml中，找到的第一个节点的值。不考虑位置
     * 从匹配选择的当前节点选择文档中的节点，而不考虑它们的位置，也就是相对位置。
     * 当存在多个同名节点时，总是获取的最前面节点对应的值
     * 比如：
     * <test>  <--可有可无，对结果无影响
     * <cmd>vpsinit</cmd><ip>15.65.110.1</ip>
     * <mac>00:15:17:44:0a:9a</mac><cmd>vpsinit2</cmd>
     * </test>
     * 中，
     * 1. 查找节点名称为 cmd 的值，xpath此时value等于vpsinit，函数返回true
     * 2. 存在两个cmd，但获取的始终是第一个cmd
     * 3. 查找不存在的节点，xpath总是返回空，函数返回 false
     */
    bool getValue(const std::string &src, const std::string &key,
                  std::string *value);

    /*
     选取此节点的所有子节点。
     比如，以下XML中
     <books>
     <book>
     <name>Codebook1</name>
     <price>13.02</price>
     <year>2005</year>
     </book>
     <book>
     <name>Codebook2</name>
     <price>14.12</price>
     <year>2005</year>
     </book>
     </books>

     选取第二个book节点，查找name，会返回Codebook2
     */
    bool getValue(const pugi::xml_node &src, const std::string &key,
                  std::string *value);

    template<class T>
    std::string getValue(const T &src, const std::string &key,
                         value_mode_t mode = VM_STRICT) {
        std::string value;
        const bool ret = getValue(src, key, &value);

        if (mode == VM_STRICT && !ret)  // 严格模式，并且获取失败，则抛出异常
            ThrowHappyException("Node mismatched.");
        else
            return value;
    }

    template<class T>
    bool getValueAsBool(const T &src, const std::string &key,
                        value_mode_t mode = VM_STRICT) {
        std::string value(getValue(src, key, mode));
        value = toLower(value);

        if (isDigit(value))
            return (value != "0");
        else if (isAlpha(value))
            return (value == "true");
        else
            return false;
    }

    template<class T>
    int32_t getValueAsInt32(const T &src, const std::string &key,
                            value_mode_t mode = VM_STRICT) {
        const std::string value(getValue(src, key, mode));
        return stoi(value);
    }

    template<class T>
    int64_t getValueAsInt64(const T &src, const std::string &key,
                            value_mode_t mode = VM_STRICT) {
        const std::string value(getValue(src, key, mode));
        return stol(value);
    }

    template<class T>
    double getValueAsDouble(const T &src, const std::string &key,
                            value_mode_t mode = VM_STRICT) {
        const std::string value(getValue(src, key, mode));
        return stod(value);
    }

    std::string getTxtValue(const pugi::xml_node &node,
                            value_mode_t mode = VM_STRICT);

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_XML_H_
