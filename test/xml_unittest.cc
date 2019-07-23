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
#include <happycpp/filesys.h>
#include <happycpp/xml.h>

namespace hhxml = happycpp::hcxml;
namespace hhfilesys = happycpp::hcfilesys;

const std::string xml("<books>"
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

const std::string pretty_xml("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
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

TEST(HCXML_UNITTEST, SpecNodeSize) {
  ASSERT_EQ(2U, hhxml::SpecNodeSize(xml, "/books/book"));
}

TEST(HCXML_UNITTEST, Validate) {
  ASSERT_FALSE(hhxml::Validate("test"));
  ASSERT_TRUE(hhxml::Validate(xml));
}

TEST(HCXML_UNITTEST, ToStr) {
  pugi::xml_document doc;

  ASSERT_TRUE(doc.load_string(xml.c_str()));
  ASSERT_EQ(pretty_xml, hhxml::ToStr(&doc));
}

TEST(HCXML_UNITTEST, Format) {
  ASSERT_EQ(pretty_xml, hhxml::Format(xml));
}

TEST(HCXML_UNITTEST, LoadFromFile) {
  const std::string xml_file("test.xml");
  ASSERT_NO_THROW(hhfilesys::WriteFile(xml_file, xml));
  ASSERT_EQ(pretty_xml, hhxml::LoadFromFile(xml_file));
  bfs::remove(xml_file);
}

TEST(HCXML_UNITTEST, GetValue_string) {
  const std::string name1(hhxml::GetValue(xml, "name"));
  EXPECT_STREQ("Codebook1", name1.c_str());

  const std::string price1(hhxml::GetValue(xml, "price"));
  EXPECT_STREQ("13.02", price1.c_str());

  EXPECT_DOUBLE_EQ(13.02, hhxml::GetValueAsDouble(xml, "price"));
  EXPECT_EQ(2005, hhxml::GetValueAsInt32(xml, "year"));
  EXPECT_EQ(2005, hhxml::GetValueAsInt64(xml, "year"));

  EXPECT_TRUE(hhxml::GetValueAsBool(xml, "sale1"));
  EXPECT_FALSE(hhxml::GetValueAsBool(xml, "sale2"));
}

TEST(HCXML_UNITTEST, GetValue_node) {
  pugi::xml_document doc;
  doc.load(xml.c_str());
  pugi::xml_node books = doc.child("books");

  pugi::xml_node book1 = books.first_child();

  const std::string name1(hhxml::GetValue(book1, "name"));
  EXPECT_STREQ("Codebook1", name1.c_str());

  const std::string price1(hhxml::GetValue(book1, "price"));
  EXPECT_STREQ("13.02", price1.c_str());

  EXPECT_DOUBLE_EQ(13.02, hhxml::GetValueAsDouble(book1, "price"));
  EXPECT_EQ(2005, hhxml::GetValueAsInt32(book1, "year"));
  EXPECT_EQ(2005, hhxml::GetValueAsInt64(book1, "year"));

  EXPECT_TRUE(hhxml::GetValueAsBool(book1, "sale1"));
  EXPECT_FALSE(hhxml::GetValueAsBool(book1, "sale2"));

  pugi::xml_node book2 = book1.next_sibling();

  const std::string name2(hhxml::GetValue(book2, "name"));
  EXPECT_STREQ("Codebook2", name2.c_str());

  const std::string price2(hhxml::GetValue(book2, "price"));
  EXPECT_STREQ("14.12", price2.c_str());

  EXPECT_DOUBLE_EQ(14.12, hhxml::GetValueAsDouble(book2, "price"));
  EXPECT_EQ(2005, hhxml::GetValueAsInt32(book2, "year"));
  EXPECT_EQ(2005, hhxml::GetValueAsInt64(book2, "year"));

  EXPECT_FALSE(hhxml::GetValueAsBool(book2, "sale1"));
  EXPECT_TRUE(hhxml::GetValueAsBool(book2, "sale2"));
}

TEST(HCXML_UNITTEST, GetTxtValue) {
  pugi::xml_document doc;
  doc.load(xml.c_str());
  pugi::xml_node name_node = doc.child("books").first_child().child("name");

  EXPECT_EQ("Codebook1", hhxml::GetTxtValue(name_node));
}

