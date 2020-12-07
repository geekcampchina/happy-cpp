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

#ifndef INCLUDE_HAPPYCPP_ALGORITHM_VERSION_H_
#define INCLUDE_HAPPYCPP_ALGORITHM_VERSION_H_

#include <cstdio>
#include <string>
#include <iostream>
#include "happycpp/common.h"

namespace happycpp {

    namespace hcalgorithm {

        namespace hcversion {

            class Version {
            public:
                explicit Version(const std::string &version) {
                    SplitVersion(version);
                }

                ~Version() = default;

                bool operator<(const Version &ver) const {
                    if (major_ < ver.major_)
                        return true;

                    if (minor_ < ver.minor_)
                        return true;

                    if (revision_ < ver.revision_)
                        return true;

                    if (build_ < ver.build_)
                        return true;

                    return false;
                }

                bool operator<=(const Version &ver) const {
                    if (major_ <= ver.major_)
                        return true;

                    if (minor_ <= ver.minor_)
                        return true;

                    if (revision_ <= ver.revision_)
                        return true;

                    if (build_ <= ver.build_)
                        return true;

                    return false;
                }

                bool operator==(const Version &ver) const {
                    return major_ == ver.major_
                           && minor_ == ver.minor_
                           && revision_ == ver.revision_
                           && build_ == ver.build_;
                }

                bool operator>(const Version &ver) const {
                    if (major_ > ver.major_)
                        return true;

                    if (minor_ > ver.minor_)
                        return true;

                    if (revision_ > ver.revision_)
                        return true;

                    if (build_ > ver.build_)
                        return true;

                    return false;
                }

                bool operator>=(const Version &ver) const {
                    if (major_ >= ver.major_)
                        return true;

                    if (minor_ >= ver.minor_)
                        return true;

                    if (revision_ >= ver.revision_)
                        return true;

                    if (build_ >= ver.build_)
                        return true;

                    return false;
                }

                friend std::ostream &operator<<(std::ostream &stream, const Version &ver) {
                    stream << ver.major_ << '.'
                           << ver.minor_ << '.'
                           << ver.revision_ << '.'
                           << ver.build_;
                    return stream;
                }

                uint32_t major() const {
                    return major_;
                }

                uint32_t minor() const {
                    return minor_;
                }

                uint32_t revision() const {
                    return revision_;
                }

                uint32_t build() const {
                    return build_;
                }

            private:
                void SplitVersion(const std::string &ver) {
                    std::sscanf(ver.c_str(), "%d.%d.%d.%d", &major_, &minor_, &revision_,
                                &build_);

                    HAPPY_ASSERT(major_ >= 0);
                    HAPPY_ASSERT(minor_ >= 0);
                    HAPPY_ASSERT(revision_ >= 0);
                    HAPPY_ASSERT(build_ >= 0);
                }

            private:
                uint32_t major_{};
                uint32_t minor_{};
                uint32_t revision_{};
                uint32_t build_{};
            };

        } /* namespace hcversion */

    } /* namespace hcalgorithm */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_ALGORITHM_VERSION_H_
