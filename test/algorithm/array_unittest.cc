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
#include "happycpp/algorithm/array.h"
#include <algorithm>

namespace hhharray = happycpp::hcalgorithm::hcarray;

TEST(HCARRAY_UNITTEST, Exists) {
    std::vector<std::string> v1{"abc", "def", "ghi"};

    EXPECT_TRUE(hhharray::Exists(v1, std::string("def")));

    std::vector<int> v2{123, 456, 789};

    EXPECT_TRUE(hhharray::Exists(v2, 789));

    std::vector<double> v3{123.0, 456.0, 789.0};

    EXPECT_TRUE(hhharray::Exists(v3, static_cast<double>(789.0)));

    std::vector<uint64_t> v4{123, 456, 789};

    EXPECT_TRUE(hhharray::Exists(v4, static_cast<uint64_t>(789)));
}

TEST(HCARRAY_UNITTEST, Find) {
    std::vector<std::string>::iterator it1;
    std::vector<std::string> v1{"abc", "def", "ghi"};

    EXPECT_TRUE(hhharray::Find(&v1, std::string("def"), &it1));
    EXPECT_STREQ("def", (*it1).c_str());

    std::vector<int>::iterator it2;
    std::vector<int> v2{111, 222, 333};

    EXPECT_TRUE(hhharray::Find(&v2, 333, &it2));
    EXPECT_EQ(333, *it2);

    std::vector<double>::iterator it3;
    std::vector<double> v3{111.0, 222.0, 333.0};

    EXPECT_TRUE(hhharray::Find(&v3, static_cast<double>(333.0), &it3));
    EXPECT_EQ(333.0, *it3);

    std::vector<uint64_t>::iterator it4;
    std::vector<uint64_t> v4{111, 222, 333};

    EXPECT_TRUE(hhharray::Find(&v4, static_cast<uint64_t>(333), &it4));
    EXPECT_EQ(333U, *it4);
}

TEST(HCARRAY_UNITTEST, Unique) {
    std::vector<int> v1{1, 2, 3, 1, 5, 3, 4, 6, 4, 7};
    std::vector<int> v2{1, 2, 3, 4, 5, 6, 7};

    hhharray::Unique(&v1);

    EXPECT_TRUE(v1 == v2);
}
