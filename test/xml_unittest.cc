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

#include <gtest/gtest.h>
#include "happycpp/filesys.h"
#include "happycpp/xml.h"

namespace hhxml = happycpp::hcxml;
namespace hhfilesys = happycpp::hcfilesys;

const std::string xml("<books>" // NOLINT
                      "<book>"
                      "<name>Codebook1</name>"
                      "<price>13.02</price>"
                      "<year>2005</year>"
                      "<sale1>true</sale1>"
                      "<sale2>0</sale2>"
                      "</book>"
                      "<book>"
                      "<name>Codebook2</name>"
                      "<price>14.12</price>"
                      "<year>2005</year>"
                      "<sale>false</sale>"
                      "<sale1>false</sale1>"
                      "<sale2>1</sale2>"
                      "</book>"
                      "</books>");

const std::string pretty_xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n" // NOLINT
                             "<books>\n"
                             "\t<book>\n"
                             "\t\t<name>Codebook1</name>\n"
                             "\t\t<price>13.02</price>\n"
                             "\t\t<year>2005</year>\n"
                             "\t\t<sale1>true</sale1>\n"
                             "\t\t<sale2>0</sale2>\n"
                             "\t</book>\n"
                             "\t<book>\n"
                             "\t\t<name>Codebook2</name>\n"
                             "\t\t<price>14.12</price>\n"
                             "\t\t<year>2005</year>\n"
                             "\t\t<sale>false</sale>\n"
                             "\t\t<sale1>false</sale1>\n"
                             "\t\t<sale2>1</sale2>\n"
                             "\t</book>\n"
                             "</books>\n");

TEST(HCXML_UNITTEST, SpecNodeSize) { // NOLINT
    EXPECT_EQ(2U, hhxml::specNodeSize(xml, "/books/book"));
}

TEST(HCXML_UNITTEST, Validate) { // NOLINT
    EXPECT_FALSE(hhxml::validate("test"));
    EXPECT_TRUE(hhxml::validate(xml));
}

TEST(HCXML_UNITTEST, ToStr) { // NOLINT
    pugi::xml_document doc;

    EXPECT_TRUE(doc.load_string(xml.c_str()));
    EXPECT_EQ(pretty_xml, hhxml::toStr(&doc));
}

TEST(HCXML_UNITTEST, Format) { // NOLINT
    EXPECT_EQ(pretty_xml, hhxml::format(xml));
}

TEST(HCXML_UNITTEST, LoadFromFile) { // NOLINT
    const std::string xml_file("test.xml");
    EXPECT_NO_THROW(hhfilesys::writeFile(xml_file, xml));
    EXPECT_EQ(pretty_xml, hhxml::loadFromFile(xml_file));
    bfs::remove(xml_file);
}

TEST(HCXML_UNITTEST, GetValueString) { // NOLINT
    const std::string name1(hhxml::getValue(xml, "name"));
    EXPECT_STREQ("Codebook1", name1.c_str());

    const std::string price1(hhxml::getValue(xml, "price"));
    EXPECT_STREQ("13.02", price1.c_str());

    EXPECT_DOUBLE_EQ(13.02, hhxml::getValueAsDouble(xml, "price"));
    EXPECT_EQ(2005, hhxml::getValueAsInt32(xml, "year"));
    EXPECT_EQ(2005, hhxml::getValueAsInt64(xml, "year"));

    EXPECT_TRUE(hhxml::getValueAsBool(xml, "sale1"));
    EXPECT_FALSE(hhxml::getValueAsBool(xml, "sale2"));
}

TEST(HCXML_UNITTEST, GetValueNode) { // NOLINT
    pugi::xml_document doc;
    doc.load(xml.c_str());
    pugi::xml_node books = doc.child("books");

    pugi::xml_node book1 = books.first_child();

    const std::string name1(hhxml::getValue(book1, "name"));
    EXPECT_STREQ("Codebook1", name1.c_str());

    const std::string price1(hhxml::getValue(book1, "price"));
    EXPECT_STREQ("13.02", price1.c_str());

    EXPECT_DOUBLE_EQ(13.02, hhxml::getValueAsDouble(book1, "price"));
    EXPECT_EQ(2005, hhxml::getValueAsInt32(book1, "year"));
    EXPECT_EQ(2005, hhxml::getValueAsInt64(book1, "year"));

    EXPECT_TRUE(hhxml::getValueAsBool(book1, "sale1"));
    EXPECT_FALSE(hhxml::getValueAsBool(book1, "sale2"));

    pugi::xml_node book2 = book1.next_sibling();

    const std::string name2(hhxml::getValue(book2, "name"));
    EXPECT_STREQ("Codebook2", name2.c_str());

    const std::string price2(hhxml::getValue(book2, "price"));
    EXPECT_STREQ("14.12", price2.c_str());

    EXPECT_DOUBLE_EQ(14.12, hhxml::getValueAsDouble(book2, "price"));
    EXPECT_EQ(2005, hhxml::getValueAsInt32(book2, "year"));
    EXPECT_EQ(2005, hhxml::getValueAsInt64(book2, "year"));

    EXPECT_FALSE(hhxml::getValueAsBool(book2, "sale1"));
    EXPECT_TRUE(hhxml::getValueAsBool(book2, "sale2"));
}

TEST(HCXML_UNITTEST, GetTxtValue) { // NOLINT
    pugi::xml_document doc;
    doc.load(xml.c_str());
    pugi::xml_node name_node = doc.child("books").first_child().child("name");

    EXPECT_EQ("Codebook1", hhxml::getTxtValue(name_node));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
