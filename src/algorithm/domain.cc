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

#include "happycpp/algorithm/domain.h"
#include "happycpp/algorithm/hcstring.h"
#include <vector>

using std::vector;

namespace happycpp {

    namespace hcalgorithm {

        namespace hcdomain {

// 判断域名块是否是有字母、数字以及不在首尾的连字符(中横线)组成，
// 比如abc.com中的abc
            HAPPYCPP_SHARED_LIB_API bool IsDomainBlock(const std::string &b) {
                if (b.empty())
                    return false;

                // 首尾字符为中横线
                if (b[0] == '-' || b[b.size() - 1] == '-')
                    return false;

                // 判断是否是有字母、数字以及连字符(中横线)组成
                for (const auto x : b) {
                    if (!isalnum(x) && x != '-')
                        return false;
                }

                return true;
            }

            HAPPYCPP_SHARED_LIB_API bool IsDomain(const std::string &s) {
                const size_t kMaxSize = 80;

                if (s.empty()  // 空
                    || s.size() > kMaxSize  // 长度超过80
                    || !hcstring::Find(s, ".")  // 没有包含点
                    || hcstring::Find(s, ".."))  // 包含连续的多个点
                    return false;

                std::vector<std::string> v_domain_block;
                hcstring::Split(s, &v_domain_block, ".");

                // 标准域名至少是abc.com这样，包含两个域名块
                if (v_domain_block.size() < 2)
                    return false;

                // *.abc.com泛域名，包含至少3个域名块，以星号开头，则移除星号
                if (v_domain_block.size() >= 3 && v_domain_block.front() == "*")
                    v_domain_block.erase(v_domain_block.begin());

                // 英文地理顶级域名，域名最后部分，只能由字母组成
                // 中文域名，域名最后部分，由字母、数字以及中横线组成，
                // 比如xn--zfr9ax31bh2evqaq74ajw1bsu0b.xn--fiqs8s
                const std::string geographic_top_level_domain(v_domain_block.back());

                // 移除数组中，最后一个
                v_domain_block.pop_back();
                if (!IsDomainBlock(geographic_top_level_domain))
                    return false;

                //  除域名最后的国家代码以外
                for (const auto& x : v_domain_block) {
                    if (!IsDomainBlock(x))
                        return false;
                }

                return true;
            }

        } /* namespace hcdomain */

    } /* namespace hcalgorithm */

} /* namespace happycpp */
