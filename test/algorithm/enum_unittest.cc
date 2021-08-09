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
#include "happycpp/algorithm/enum.hpp"

using namespace std;
namespace hhhenum = happycpp::hcalgorithm::henum;

enum Color : std::uint8_t {
    RED = 1,
    GREEN = 2,
    WHITE = 3,
    PINK = 4
};

TEST(HCENUM_UNITTEST, EnumPlus) { // NOLINT
    map<Color, string> m = {
            DEFINE_ENUM_MAP_ITEM(RED),
            DEFINE_ENUM_MAP_ITEM(GREEN),
            DEFINE_ENUM_MAP_ITEM(WHITE),
            DEFINE_ENUM_MAP_ITEM(PINK),
    };

    hhhenum::EnumPlus<Color, uint8_t> ep(m);

    EXPECT_EQ(ep.toEnum(3), WHITE);
    EXPECT_THROW(ep.toEnum(255), happycpp::HappyException);
    EXPECT_TRUE(ep.exists(3));
    EXPECT_FALSE(ep.exists(255));

    EXPECT_STREQ(ep.toName(RED).c_str(), "RED");
    EXPECT_STREQ(ep.toName(WHITE).c_str(), "WHITE");
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
