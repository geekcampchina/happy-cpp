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

TEST(HCHTTP_UNITTEST, hcurl_decode) {
    ASSERT_EQ("1234%^&5345+- =abc",
              hhhttp::hcurl::decode("1234%25%5E%265345%2B-+%3Dabc"));
}

TEST(HCHTTP_UNITTEST, hcurl_encode) {
    ASSERT_EQ("1234%25%5E%265345%2B-+%3Dabc",
              hhhttp::hcurl::encode("1234%^&5345+- =abc"));
}

TEST(HCHTTP_UNITTEST, parse_http_msg_request) {
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

    ASSERT_NE(nullptr, hm);
    ASSERT_EQ(hhhttp::HTTP_MSG_REQUEST, hm->type());
    ASSERT_EQ(hhhttp::HTTP_METHOD_POST, hm->method());
    ASSERT_EQ("/index?arg=test", hm->requestUrl());
    ASSERT_EQ("/index", hm->url());
    ASSERT_EQ("arg=test", hm->args());
    ASSERT_EQ("HTTP/1.0", hm->version());

    ASSERT_EQ("www.example.com", hm->header(hhhttp::HTTP_MREQF_HOST));
    ASSERT_EQ("*/*", hm->header(hhhttp::HTTP_MREQF_ACCEPT));
    ASSERT_EQ("12", hm->header(hhhttp::HTTP_MCOMF_CONTENT_LENGTH));
    ASSERT_EQ("application/x-www-form-urlencoded",
              hm->header(hhhttp::HTTP_MCOMF_CONTENT_TYPE));
    ASSERT_EQ("", hm->header(hhhttp::HTTP_MREQF_COOKIE));

    ASSERT_EQ("hello world\n", hm->body());
}

TEST(HCHTTP_UNITTEST, parse_http_msg_response) {
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

    ASSERT_EQ(1, hm.use_count());
    ASSERT_EQ(hhhttp::HTTP_MSG_RESPONSE, hm->type());
    ASSERT_EQ("HTTP/1.1", hm->version());
    ASSERT_EQ(416U, hm->status());
    ASSERT_EQ("Requested Range Not Satisfiable", hm->reasonPhrase());

    ASSERT_EQ("nginx/1.8.0", hm->header(hhhttp::HTTP_MRESF_SERVER));
    ASSERT_EQ("Sat, 26 Dec 2015 03:36:50 GMT",
              hm->header(hhhttp::HTTP_MCOMF_DATE));
    ASSERT_EQ("text/html", hm->header(hhhttp::HTTP_MCOMF_CONTENT_TYPE));
    ASSERT_EQ("212", hm->header(hhhttp::HTTP_MCOMF_CONTENT_LENGTH));
    ASSERT_EQ("keep-alive", hm->header(hhhttp::HTTP_MCOMF_CONNECTION));
    ASSERT_EQ("", hm->header(hhhttp::HTTP_MCOMF_CACHE_CONTROL));

    ASSERT_EQ("", hm->body());
}

TEST(HCHTTP_UNITTEST, get_header_info) {
    hhhttp::HttpResponseMsgPtr hm =
            hhhttp::getHeaderInfo("http://127.0.0.1:8887");

    ASSERT_EQ(1, hm.use_count());
    ASSERT_EQ(hhhttp::HTTP_MSG_RESPONSE, hm->type());
    ASSERT_EQ("HTTP/1.0", hm->version());
    ASSERT_EQ(200U, hm->status());
    EXPECT_EQ("OK", hm->reasonPhrase());
    EXPECT_EQ("text/plain", hm->header(hhhttp::HTTP_MCOMF_CONTENT_TYPE));
    EXPECT_EQ("100", hm->header(hhhttp::HTTP_MCOMF_CONTENT_LENGTH));
}
