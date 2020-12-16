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
#include "happycpp/http.h"

namespace hhhttp = happycpp::hchttp;

TEST(HCHTTP_UNITTEST, HcurlDecode) { // NOLINT
    EXPECT_EQ("1234%^&5345+- =abc",
              hhhttp::hcurl::decode("1234%25%5E%265345%2B-+%3Dabc"));
}

TEST(HCHTTP_UNITTEST, HcurlEncode) { // NOLINT
    EXPECT_EQ("1234%25%5E%265345%2B-+%3Dabc",
              hhhttp::hcurl::encode("1234%^&5345+- =abc"));
}

TEST(HCHTTP_UNITTEST, ParseHttpMsgRequest) { // NOLINT
    const std::string http("POST /index?arg=test HTTP/1.0\r\n"
                           "Host: www.example.com\r\n"
                           "Accept: */*\r\n"
                           "Content-Length: 12\r\n"
                           "Content-Type: application/x-www-form-urlencoded\r\n"
                           "\r\n"
                           "hello world\n");

    hhhttp::HttpMsgCtx ctx;
    hhhttp::HttpRequestMsgPtr hm =
            std::dynamic_pointer_cast<hhhttp::HttpRequestMsg>(ctx.parse(http));

    EXPECT_NE(nullptr, hm);
    EXPECT_EQ(hhhttp::HTTP_MSG_REQUEST, hm->type());
    EXPECT_EQ(hhhttp::HTTP_METHOD_POST, hm->method());
    EXPECT_EQ("/index?arg=test", hm->requestUrl());
    EXPECT_EQ("/index", hm->url());
    EXPECT_EQ("arg=test", hm->args());
    EXPECT_EQ("HTTP/1.0", hm->version());

    EXPECT_EQ("www.example.com", hm->header(hhhttp::HTTP_MREQF_HOST));
    EXPECT_EQ("*/*", hm->header(hhhttp::HTTP_MREQF_ACCEPT));
    EXPECT_EQ("12", hm->header(hhhttp::HTTP_MCOMF_CONTENT_LENGTH));
    EXPECT_EQ("application/x-www-form-urlencoded",
              hm->header(hhhttp::HTTP_MCOMF_CONTENT_TYPE));
    EXPECT_EQ("", hm->header(hhhttp::HTTP_MREQF_COOKIE));

    EXPECT_EQ("hello world\n", hm->body());
}

TEST(HCHTTP_UNITTEST, ParseHttpMsgResponse) { // NOLINT
    const std::string http("HTTP/1.1 416 Requested Range Not Satisfiable\r\n"
                           "Server: nginx/1.8.0\r\n"
                           "Date: Sat, 26 Dec 2015 03:36:50 GMT\r\n"
                           "Content-Type: text/html\r\n"
                           "Content-Length: 212\r\n"
                           "Connection: keep-alive\r\n"
                           "\r\n");

    hhhttp::HttpMsgCtx ctx;
    hhhttp::HttpResponseMsgPtr hm =
            std::dynamic_pointer_cast<hhhttp::HttpResponseMsg>(ctx.parse(http));

    EXPECT_EQ(1, hm.use_count());
    EXPECT_EQ(hhhttp::HTTP_MSG_RESPONSE, hm->type());
    EXPECT_EQ("HTTP/1.1", hm->version());
    EXPECT_EQ(416U, hm->status());
    EXPECT_EQ("Requested Range Not Satisfiable", hm->reasonPhrase());

    EXPECT_EQ("nginx/1.8.0", hm->header(hhhttp::HTTP_MRESF_SERVER));
    EXPECT_EQ("Sat, 26 Dec 2015 03:36:50 GMT",
              hm->header(hhhttp::HTTP_MCOMF_DATE));
    EXPECT_EQ("text/html", hm->header(hhhttp::HTTP_MCOMF_CONTENT_TYPE));
    EXPECT_EQ("212", hm->header(hhhttp::HTTP_MCOMF_CONTENT_LENGTH));
    EXPECT_EQ("keep-alive", hm->header(hhhttp::HTTP_MCOMF_CONNECTION));
    EXPECT_EQ("", hm->header(hhhttp::HTTP_MCOMF_CACHE_CONTROL));

    EXPECT_EQ("", hm->body());
}

TEST(HCHTTP_UNITTEST, GetHeaderInfo) { // NOLINT
    hhhttp::HttpResponseMsgPtr hm =
            hhhttp::getHeaderInfo("http://127.0.0.1:8887");

    EXPECT_EQ(1, hm.use_count());
    EXPECT_EQ(hhhttp::HTTP_MSG_RESPONSE, hm->type());
    EXPECT_EQ("HTTP/1.0", hm->version());
    EXPECT_EQ(200U, hm->status());
    EXPECT_EQ("OK", hm->reasonPhrase());
    EXPECT_EQ("text/plain", hm->header(hhhttp::HTTP_MCOMF_CONTENT_TYPE));
    EXPECT_EQ("100", hm->header(hhhttp::HTTP_MCOMF_CONTENT_LENGTH));
}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

