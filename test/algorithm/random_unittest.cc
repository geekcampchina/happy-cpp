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
#include "happycpp/algorithm/random.h"
#include "happycpp/algorithm/array.h"
#include "happycpp/algorithm/hctime.h"

namespace hhharray = happycpp::hcalgorithm::hcarray;
namespace hhhtime = happycpp::hcalgorithm::hctime;
namespace hhhrandom = happycpp::hcalgorithm::hcrandom;

bool CheckCharClassification(const std::string &s,
                             CharClassification cc = kPrint);

bool CheckCharClassification(const std::string &s, CharClassification cc) {
    bool ret = true;

    for (uint32_t i = 0; i < s.size(); ++i) {
        char c = s[i];

        switch (cc) {
            case kAlnum:
                ret = isalnum(c);
                break;
            case kAlpha:
                ret = isalpha(c);
                break;
            case kDigit:
                ret = isdigit(c);
                break;
            case kPrint:
                ret = (!isspace(c) && isprint(c));
                break;
            default:
                break;
        }

        if (!ret)
            return false;
    }

    return true;
}

TEST(HCRANDOM_UNITTEST, GetRandom_int64) {
    std::vector<int64_t> v;

    for (int64_t i = 0; i < 100; ++i) {
        hhhtime::HappySleep(1);
        v.push_back(hhhrandom::GetRandom());
    }

    hhharray::Unique(&v);
    const size_t v_unique_size = v.size();
    const double ratio = v_unique_size / 100.0;

    // ratio >= 90%
    EXPECT_GE(ratio, 0.90);
}

TEST(HCRANDOM_UNITTEST, GetRandom_str) {
    const std::string s1(hhhrandom::GenRandom(11, kAlnum));
    EXPECT_EQ(11U, s1.size());
    EXPECT_TRUE(CheckCharClassification(s1, kAlnum));

    const std::string s2(hhhrandom::GenRandom(12, kAlpha));
    EXPECT_EQ(12U, s2.size());
    EXPECT_TRUE(CheckCharClassification(s2, kAlpha));

    const std::string s3(hhhrandom::GenRandom(13, kDigit));
    EXPECT_EQ(13U, s3.size());
    EXPECT_TRUE(CheckCharClassification(s3, kDigit));

    const std::string s4(hhhrandom::GenRandom(14, kPrint));
    EXPECT_EQ(14U, s4.size());
    EXPECT_TRUE(CheckCharClassification(s4, kPrint));
}
