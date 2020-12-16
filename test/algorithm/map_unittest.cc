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
#include "happycpp/algorithm/map.h"

namespace hhhmap = happycpp::hcalgorithm::hcmap;

TEST(HCMAP_UNITTEST, FindByValue) { // NOLINT
    std::map<std::string, int> log_level_map{
            {"trace", 0},
            {"debug", 1},
            {"info",  2},
            {"warn",  3},
            {"error", 4},
            {"fatal", 5},
            {"off",   6}
    };

    auto it1 =
            hhhmap::findByValue(log_level_map, 4);

    EXPECT_EQ("error", it1->first);
    EXPECT_EQ(4, it1->second);

    auto it2 =
            hhhmap::findByValue(log_level_map, 10);

    EXPECT_EQ(log_level_map.end(), it2);
}


TEST(HCMAP_UNITTEST, SwapKeyValue) { // NOLINT
    std::map<std::string, int> log_level_map1{
            {"trace", 0},
            {"debug", 1},
            {"info",  2},
            {"warn",  3},
            {"error", 4},
            {"fatal", 5},
            {"off",   6}
    };
    const size_t ele_size = log_level_map1.size();

    std::map<int, std::string> log_level_map2;
    hhhmap::swapKeyValue(log_level_map1, &log_level_map2);

    EXPECT_EQ(ele_size, log_level_map2.size());

    for (const auto &it : log_level_map1) {
        auto find_it =
                hhhmap::findByValue(log_level_map2, it.first);

        EXPECT_NE(log_level_map2.end(), find_it);
        EXPECT_EQ(it.first, find_it->second);
    }
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}
