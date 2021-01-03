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

#include "happycpp/xml.h"

using pugi::xml_parse_result;
using pugi::xpath_node_set;
using std::ostringstream;

namespace happycpp::hcxml {

    /* 在源xml中查找指定的节点，返回匹配的节点数量
     * xpath表示指定的节点路径 */
    size_t specNodeSize(const std::string &xml, const std::string &xpath) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(xml.c_str());

        if (result) {
            /* 未使用 select_single_node 是因为
             * select_nodes可以返回匹配的节点数量，判断是否有匹配的结果 */
            const xpath_node_set nodes = doc.select_nodes(xpath.c_str());
            return nodes.size();
        }

        return 0;
    }

    std::string toStr(pugi::xml_document *doc, const std::string &indent) {
        ostringstream ss;

        if (doc) {
            pugi::xml_node decl = doc->prepend_child(pugi::node_declaration);
            decl.append_attribute("version") = "1.0";
            decl.append_attribute("encoding") = "UTF-8";

            doc->save(ss, indent.c_str());
            return ss.str();
        }

        return "";
    }

    std::string loadFromFile(const std::string &f) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_file(f.c_str());

        if (result)
            return toStr(&doc);

        return "";
    }

    std::string format(const std::string &xml, const std::string &indent) {
        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(xml.c_str());

        if (result)
            return toStr(&doc, indent);

        return xml;
    }

    /* 验证字符串是否是xml格式 */
    bool validate(const std::string &xml) {
        pugi::xml_document doc;
        return doc.load_string(xml.c_str());
    }

    bool getValue(const pugi::xml_node &src, const std::string &key,
                  std::string *value) {
        if (key.empty())
            return false;

        const std::string &xpath_query(key);
        /* 未使用 select_single_node 是因为
         * select_nodes可以返回匹配的节点数量，判断是否有匹配的结果 */
        pugi::xpath_node_set nodes = src.select_nodes(xpath_query.c_str());

        if (nodes.empty())
            return false;

        *value = std::string(nodes.first().node().child_value());
        return true;
    }

    bool getValue(const std::string &src, const std::string &key,
                  std::string *value) {
        if (src.empty() || key.empty())
            return false;

        pugi::xml_document doc;
        pugi::xml_parse_result result = doc.load_string(src.c_str());

        if (!result)
            return false;

        const std::string xpath_query("//" + key);
        /* 未使用 select_single_node 是因为
         * select_nodes可以返回匹配的节点数量，判断是否有匹配的结果 */
        xpath_node_set nodes = doc.select_nodes(xpath_query.c_str());

        if (nodes.empty())
            return false;

        *value = std::string(nodes.first().node().child_value());

        return true;
    }

    std::string getTxtValue(const pugi::xml_node &node, value_mode_t mode) {
        std::string v(node.text().as_string());

        if (mode == VM_STRICT && v.empty())
            ThrowHappyException("Can not get the value of xml node.");

        return v;
    }

} /* namespace happycpp */
