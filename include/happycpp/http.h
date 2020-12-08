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

#ifndef INCLUDE_HAPPYCPP_HTTP_H_
#define INCLUDE_HAPPYCPP_HTTP_H_

#include <string>
#include <map>
#include <memory>
#include <vector>

namespace happycpp {

    namespace hchttp {

        //! HTTP 请求方法枚举
        /*!
         http://www.w3.org/Protocols/rfc2616/rfc2616-sec9.html
         */
        typedef enum {
            INVALID_HTTP_METHOD = -1,  /// 无效的 HTTP 方法
            HTTP_METHOD_CONNECT,
            HTTP_METHOD_DELETE,
            HTTP_METHOD_GET,
            HTTP_METHOD_HEAD,
            HTTP_METHOD_POST,
            HTTP_METHOD_PUT,
            HTTP_METHOD_TRACE
        } HttpMethodType;

        //! HTTP 消息字段枚举
        /*!
         non-standard(nonstd)，非标准
         HTTP Message Common field(http_mcomf)，HTTP 消息通用字段
         HTTP Message Request field(http_mreqf)，HTTP 消息请求字段
         HTTP Message Response field(http_mresf)，HTTP 消息响应字段

         http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4
         https://en.wikipedia.org/wiki/List_of_HTTP_header_fields
         */
        typedef enum {
            INVALID_HTTP_MSG_FIELD = -1,  /// 无效的 HTTP 字段
            HTTP_MCOMF_CACHE_CONTROL,
            HTTP_MCOMF_CONNECTION,
            HTTP_MCOMF_CONTENT_LENGTH,
            HTTP_MCOMF_CONTENT_MD5,
            HTTP_MCOMF_CONTENT_TYPE,
            HTTP_MCOMF_DATE,
            HTTP_MCOMF_PRAGMA,
            HTTP_MCOMF_UPGRADE,
            HTTP_MCOMF_VIA,
            HTTP_MCOMF_WARNING,
            HTTP_MREQF_ACCEPT,
            HTTP_MREQF_ACCEPT_CHARSET,
            HTTP_MREQF_ACCEPT_ENCODING,
            HTTP_MREQF_ACCEPT_LANGUAGE,
            HTTP_MREQF_ACCEPT_DATETIME,
            HTTP_MREQF_AUTHORIZATION,
            HTTP_MREQF_COOKIE,
            HTTP_MREQF_EXPECT,
            HTTP_MREQF_FROM,
            HTTP_MREQF_HOST,
            HTTP_MREQF_IF_MATCH,
            HTTP_MREQF_IF_MODIFIED_SINCE,
            HTTP_MREQF_IF_NONE_MATCH,
            HTTP_MREQF_IF_RANGE,
            HTTP_MREQF_IF_UNMODIFIED_SINCE,
            HTTP_MREQF_MAX_FORWARDS,
            HTTP_MREQF_ORIGIN,
            HTTP_MREQF_PROXY_AUTHORIZATION,
            HTTP_MREQF_RANGE,
            HTTP_MREQF_REFERER,
            HTTP_MREQF_TE,
            HTTP_MREQF_USER_AGENT,
            HTTP_MREQF_NONSTD_X_REQUESTED_WITH,
            HTTP_MREQF_NONSTD_DNT,
            HTTP_MREQF_NONSTD_X_FORWARDED_FOR,
            HTTP_MREQF_NONSTD_X_FORWARDED_HOST,
            HTTP_MREQF_NONSTD_X_FORWARDED_PROTO,
            HTTP_MREQF_NONSTD_FRONT_END_HTTPS,
            HTTP_MREQF_NONSTD_X_HTTP_METHOD_OVERRIDE,
            HTTP_MREQF_NONSTD_X_ATT_DEVICEID,
            HTTP_MREQF_NONSTD_X_WAP_PROFILE,
            HTTP_MREQF_NONSTD_PROXY_CONNECTION,
            HTTP_MREQF_NONSTD_X_UIDH,
            HTTP_MREQF_NONSTD_X_CSRF_TOKEN,
            HTTP_MRESF_ACCESS_CONTROL_ALLOW_ORIGIN,
            HTTP_MRESF_ACCEPT_PATCH,
            HTTP_MRESF_ACCEPT_RANGES,
            HTTP_MRESF_AGE,
            HTTP_MRESF_ALLOW,
            HTTP_MRESF_CONTENT_DISPOSITION,
            HTTP_MRESF_CONTENT_ENCODING,
            HTTP_MRESF_CONTENT_LANGUAGE,
            HTTP_MRESF_CONTENT_LOCATION,
            HTTP_MRESF_CONTENT_RANGE,
            HTTP_MRESF_ETAG,
            HTTP_MRESF_EXPIRES,
            HTTP_MRESF_LAST_MODIFIED,
            HTTP_MRESF_LINK,
            HTTP_MRESF_LOCATION,
            HTTP_MRESF_P3P,
            HTTP_MRESF_PROXY_AUTHENTICATE,
            HTTP_MRESF_PUBLIC_KEY_PINS,
            HTTP_MRESF_REFRESH,
            HTTP_MRESF_RETRY_AFTER,
            HTTP_MRESF_SERVER,
            HTTP_MRESF_SET_COOKIE,
            HTTP_MRESF_STATUS,
            HTTP_MRESF_STRICT_TRANSPORT_SECURITY,
            HTTP_MRESF_TRAILER,
            HTTP_MRESF_TRANSFER_ENCODING,
            HTTP_MRESF_TSV,
            HTTP_MRESF_VARY,
            HTTP_MRESF_WWW_AUTHENTICATE,
            HTTP_MRESF_X_FRAME_OPTIONS,
            HTTP_MRESF_NONSTD_X_XSS_PROTECTION,
            HTTP_MRESF_NONSTD_CONTENT_SECURITY_POLICY,
            HTTP_MRESF_NONSTD_X_CONTENT_SECURITY_POLICY,
            HTTP_MRESF_NONSTD_X_WEBKIT_CSP,
            HTTP_MRESF_NONSTD_X_CONTENT_TYPE_OPTIONS,
            HTTP_MRESF_NONSTD_X_POWERED_BY,
            HTTP_MRESF_NONSTD_X_UA_COMPATIBLE,
            HTTP_MRESF_NONSTD_X_CONTENT_DURATION,
        } HttpMsgField;

        //! HTTP 消息类型枚举
        typedef enum {
            INVALID_HTTP_MSG_TYPE = -1,  /// 无效的 HTTP 消息类型
            HTTP_MSG_REQUEST,
            HTTP_MSG_RESPONSE,
        } HttpMsgType;

        //! HTTP 消息基类
        class HttpMessage {
        protected:
            static std::map<std::string, HttpMethodType> mDescHm;
            static std::map<std::string, HttpMsgField> mDescHmf;

            HttpMsgType type_; /*! 消息类型 */
            std::string version_; /*! HTTP 版本 */
            std::map<HttpMsgField, std::string> header_; /*! message-header 消息字段集合*/
            std::string body_; /*! message-body */

        protected:
            HttpMessage();

        public:
            virtual ~HttpMessage();

            HttpMethodType toHm(const std::string &k);

            HttpMsgField toHmf(const std::string &k);

            void setVersion(const std::string &v);

            void addField(HttpMsgField k, const std::string &v);

            void setBody(const std::string &v);

            HttpMsgType type();

            std::string version();
            //! 获取指定字段的值
            /*!
             * @param hmf 指定字段
             * @return 字符串 如果指定字段不存在值，则返回空
             */
            std::string header(HttpMsgField hmf);

            void header(std::map<HttpMsgField, std::string> *hmf);

            std::string body();
        };

        //! HTTP 请求消息类
        /*!
         HttpMessage 派生类
         */
        class HttpRequestMsg : public HttpMessage {
        private:
            HttpMethodType method_; /*! 请求字段 */
            /*! 请求字段。原始请求路径，包括路径和问号后面的参数 */
            std::string request_url_;
            std::string url_; /*! 请求字段。请求路径不包括问号后面的参数 */
            std::string args_; /*! 请求字段 */

        public:
            HttpRequestMsg();

            ~HttpRequestMsg() override;

            void setMethod(HttpMethodType v);

            void setRequestUrl(const std::string &v);

            void setUrl(const std::string &v);

            void setArgs(const std::string &v);

            HttpMethodType method();

            std::string requestUrl();

            std::string url();

            std::string args();
        };

        //! HTTP 响应消息类
        /*!
         HttpMessage 派生类
         */
        class HttpResponseMsg : public HttpMessage {
        private:
            uint32_t status_; /*! 响应字段。状态码 */
            std::string reasonPhrase_; /*! 响应字段。原因简述 */

        public:
            HttpResponseMsg();

            ~HttpResponseMsg() override;

            void setStatus(uint32_t v);

            void setReasonPhrase(const std::string &v);

            uint32_t status();

            std::string reasonPhrase();
        };

        typedef std::shared_ptr<HttpMessage> HttpMessagePtr;
        typedef std::shared_ptr<HttpRequestMsg> HttpRequestMsgPtr;
        typedef std::shared_ptr<HttpResponseMsg> HttpResponseMsgPtr;

        //! HttpMessage 上下文类
        /*!
         将 HTTP 消息转换为 HttpMessage 类指针

         HTTP 消息格式如下：
         @verbatim
         generic-message = start-line
         *(message-header CRLF)
         CRLF
         [ message-body ]
         start-line      = Request-Line | Status-Line
         @endverbatim

         http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4
         */
        class HttpMsgCtx {
        private:
            //! 初始化 HTTP 消息类智能指针
            /*!
             * @param http HTTP 消息原始数据
             * @return HTTP 消息类智能指针
             */
            HttpMessagePtr initHm(const std::string &http);

            //! 转换 HTTP 消息请求行
            /*! 比如，
             GET /index?arg=test HTTP/1.0
             POST /index HTTP/1.0
             */
            /*!
             * @param line 请求行字符串
             * @param hm HTTP 消息类智能指针
             */
            void parseRequestLine(const std::string &line, HttpMessagePtr hm);

            //! 转换 HTTP 消息状态行
            /*! 比如，
             HTTP/1.1 200 OK
             HTTP/1.1 416 Requested Range Not Satisfiable
             */
            /*!
             * @param line 状态行字符串
             * @param hm HTTP 消息类智能指针
             */
            void parseStatusLine(const std::string &line, HttpMessagePtr hm);

            //! 转换 HTTP 消息起始行
            /*! 比如，
             GET /index?arg=test HTTP/1.0
             POST /index HTTP/1.0
             HTTP/1.1 200 OK
             HTTP/1.1 416 Requested Range Not Satisfiable
             */
            /*!
             * @param line 起始行字符串
             * @param hm HTTP 消息类智能指针
             */
            void parseStartLine(const std::string &line, HttpMessagePtr hm);

            //! 转换 HTTP 消息头

            //! 比如，
            //! Host: www.example.com\r\n
            //! Accept: */*\r\n
            /*!
             * @param 起始行到消息体之间的字符串
             * @param hm HTTP 消息类智能指针
             */
            void parseHeader(const std::string &header, HttpMessagePtr hm);

            //! 转换 HTTP 消息体
            /*! 比如，
             Content-Type: application/x-www-form-urlencoded\r\n
             \r\n
             hello world\n

             中的"hello world"
             */
            /*!
             * @param body 消息体字符串
             * @param hm HTTP 消息类智能指针
             */
            void parseBody(const std::string &body, HttpMessagePtr hm);

        public:
            HttpMsgCtx();

            ~HttpMsgCtx() noexcept;

            //! 转换 HTTP 消息
            /*!
             HTTP 消息格式如下：
             @verbatim
             generic-message = start-line
             *(message-header CRLF)
             CRLF
             [ message-body ]
             start-line      = Request-Line | Status-Line
             @endverbatim

             http://www.w3.org/Protocols/rfc2616/rfc2616-sec4.html#sec4
             */
            /*!
             * @param http HTTP 消息原始数据
             * @return HTTP 消息类智能指针
             */
            HttpMessagePtr parse(const std::string &http);
        };

        // 根据uri，返回对应的mime type
        const std::string getMimeType(const std::string &uri,
                                      const std::string &charset);
        //! 从 URL 获取头信息
        /*!
         * @param url URL 地址
         * @return HttpResponseMsg 智能指针
         */
        HttpResponseMsgPtr getHeaderInfo(const std::string &url);

        namespace hcurl {

            std::string encode(const std::string &s);

            std::string decode(const std::string &s);

        } /* namespace hcurl */

    } /* namespace hchttp */

} /* namespace happycpp */

#endif  // INCLUDE_HAPPYCPP_HTTP_H_

