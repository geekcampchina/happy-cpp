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

#include "happycpp/http.h"
#include "happycpp/exception.h"
#include "happycpp/algorithm.h"
#include <curl/curl.h>

using happycpp::hcalgorithm::hcstring::Split;
using happycpp::hcalgorithm::hcstring::Trim;

namespace happycpp {

    namespace hchttp {

        std::map<std::string, HttpMethodType> HttpMessage::m_desc_hm = {
                {"",        INVALID_HTTP_METHOD},
                {"CONNECT", HTTP_METHOD_CONNECT},
                {"DELETE",  HTTP_METHOD_DELETE},
                {"GET",     HTTP_METHOD_GET},
                {"HEAD",    HTTP_METHOD_HEAD},
                {"POST",    HTTP_METHOD_POST},
                {"PUT",     HTTP_METHOD_PUT},
                {"TRACE",   HTTP_METHOD_TRACE}
        };

        std::map<std::string, HttpMsgField> HttpMessage::m_desc_hmf = {
                {"",                            INVALID_HTTP_MSG_FIELD},
                {"Cache-Control",               HTTP_MCOMF_CACHE_CONTROL},
                {"Connection",                  HTTP_MCOMF_CONNECTION},
                {"Content-Length",              HTTP_MCOMF_CONTENT_LENGTH},
                {"Content-MD5",                 HTTP_MCOMF_CONTENT_MD5},
                {"Content-Type",                HTTP_MCOMF_CONTENT_TYPE},
                {"Date",                        HTTP_MCOMF_DATE},
                {"Pragma",                      HTTP_MCOMF_PRAGMA},
                {"Upgrade",                     HTTP_MCOMF_UPGRADE},
                {"Via",                         HTTP_MCOMF_VIA},
                {"Warning",                     HTTP_MCOMF_WARNING},
                {"Accept",                      HTTP_MREQF_ACCEPT},
                {"Accept-Charset",              HTTP_MREQF_ACCEPT_CHARSET},
                {"Accept-Encoding",             HTTP_MREQF_ACCEPT_ENCODING},
                {"Accept-Language",             HTTP_MREQF_ACCEPT_LANGUAGE},
                {"Accept-Datetime",             HTTP_MREQF_ACCEPT_DATETIME},
                {"Authorization",               HTTP_MREQF_AUTHORIZATION},
                {"Cookie",                      HTTP_MREQF_COOKIE},
                {"Expect",                      HTTP_MREQF_EXPECT},
                {"From",                        HTTP_MREQF_FROM},
                {"Host",                        HTTP_MREQF_HOST},
                {"If-Match",                    HTTP_MREQF_IF_MATCH},
                {"If-Modified-Since",           HTTP_MREQF_IF_MODIFIED_SINCE},
                {"If-None-Match",               HTTP_MREQF_IF_NONE_MATCH},
                {"If-Range",                    HTTP_MREQF_IF_RANGE},
                {"If-Unmodified-Since",         HTTP_MREQF_IF_UNMODIFIED_SINCE},
                {"Max-Forwards",                HTTP_MREQF_MAX_FORWARDS},
                {"Origin",                      HTTP_MREQF_ORIGIN},
                {"Proxy-Authorization",         HTTP_MREQF_PROXY_AUTHORIZATION},
                {"Range",                       HTTP_MREQF_RANGE},
                {"Referer",                     HTTP_MREQF_REFERER},
                {"TE",                          HTTP_MREQF_TE},
                {"User-Agent",                  HTTP_MREQF_USER_AGENT},
                {"X_Requested_With",            HTTP_MREQF_NONSTD_X_REQUESTED_WITH},
                {"DNT",                         HTTP_MREQF_NONSTD_DNT},
                {"X_Forwarded_For",             HTTP_MREQF_NONSTD_X_FORWARDED_FOR},
                {"X_Forwarded_Host",            HTTP_MREQF_NONSTD_X_FORWARDED_HOST},
                {"X_Forwarded_Proto",           HTTP_MREQF_NONSTD_X_FORWARDED_PROTO},
                {"Front_End_Https",             HTTP_MREQF_NONSTD_FRONT_END_HTTPS},
                {"X_Http_Method_Override",      HTTP_MREQF_NONSTD_X_HTTP_METHOD_OVERRIDE},
                {"X_ATT_DeviceId",              HTTP_MREQF_NONSTD_X_ATT_DEVICEID},
                {"X_Wap_Profile",               HTTP_MREQF_NONSTD_X_WAP_PROFILE},
                {"Proxy_Connection",            HTTP_MREQF_NONSTD_PROXY_CONNECTION},
                {"X_UIDH",                      HTTP_MREQF_NONSTD_X_UIDH},
                {"X_Csrf_Token",                HTTP_MREQF_NONSTD_X_CSRF_TOKEN},
                {"Access-Control-Allow-Origin", HTTP_MRESF_ACCESS_CONTROL_ALLOW_ORIGIN},
                {"Accept-Patch",                HTTP_MRESF_ACCEPT_PATCH},
                {"Accept-Ranges",               HTTP_MRESF_ACCEPT_RANGES},
                {"Age",                         HTTP_MRESF_AGE},
                {"Allow",                       HTTP_MRESF_ALLOW},
                {"Content-Disposition",         HTTP_MRESF_CONTENT_DISPOSITION},
                {"Content-Encoding",            HTTP_MRESF_CONTENT_ENCODING},
                {"Content-Language",            HTTP_MRESF_CONTENT_LANGUAGE},
                {"Content-Location",            HTTP_MRESF_CONTENT_LOCATION},
                {"Content-Range",               HTTP_MRESF_CONTENT_RANGE},
                {"ETag",                        HTTP_MRESF_ETAG},
                {"Expires",                     HTTP_MRESF_EXPIRES},
                {"Last-Modified",               HTTP_MRESF_LAST_MODIFIED},
                {"Link",                        HTTP_MRESF_LINK},
                {"Location",                    HTTP_MRESF_LOCATION},
                {"P3P",                         HTTP_MRESF_P3P},
                {"Proxy-Authenticate",          HTTP_MRESF_PROXY_AUTHENTICATE},
                {"Public-Key-Pins",             HTTP_MRESF_PUBLIC_KEY_PINS},
                {"Refresh",                     HTTP_MRESF_REFRESH},
                {"Retry-After",                 HTTP_MRESF_RETRY_AFTER},
                {"Server",                      HTTP_MRESF_SERVER},
                {"Set-Cookie",                  HTTP_MRESF_SET_COOKIE},
                {"Status",                      HTTP_MRESF_STATUS},
                {"Strict-Transport-Security",   HTTP_MRESF_STRICT_TRANSPORT_SECURITY},
                {"Trailer",                     HTTP_MRESF_TRAILER},
                {"Transfer-Encoding",           HTTP_MRESF_TRANSFER_ENCODING},
                {"TSV",                         HTTP_MRESF_TSV},
                {"Vary",                        HTTP_MRESF_VARY},
                {"WWW-Authenticate",            HTTP_MRESF_WWW_AUTHENTICATE},
                {"X-Frame-Options",             HTTP_MRESF_X_FRAME_OPTIONS},
                {"X-XSS-Protection",            HTTP_MRESF_NONSTD_X_XSS_PROTECTION},
                {"Content-Security-Policy",     HTTP_MRESF_NONSTD_CONTENT_SECURITY_POLICY},
                {"X-Content-Security-Policy",   HTTP_MRESF_NONSTD_X_CONTENT_SECURITY_POLICY},
                {"X-WebKit-CSP",                HTTP_MRESF_NONSTD_X_WEBKIT_CSP},
                {"X-Content-Type-Options",      HTTP_MRESF_NONSTD_X_CONTENT_TYPE_OPTIONS},
                {"X-Powered-By",                HTTP_MRESF_NONSTD_X_POWERED_BY},
                {"X-UA-Compatible",             HTTP_MRESF_NONSTD_X_UA_COMPATIBLE},
                {"X-Content-Duration",          HTTP_MRESF_NONSTD_X_CONTENT_DURATION}
        };

        HttpMessage::HttpMessage()
                : type_(INVALID_HTTP_MSG_TYPE), version_(""), body_("") {
        }

        HttpMessage::~HttpMessage() {
        }

        HttpMethodType HttpMessage::to_hm(const std::string &k) {
            std::map<std::string, HttpMethodType>::const_iterator it = m_desc_hm.find(k);

            if (it == m_desc_hm.end())
                return INVALID_HTTP_METHOD;

            return it->second;
        }

        HttpMsgField HttpMessage::to_hmf(const std::string &k) {
            std::map<std::string, HttpMsgField>::const_iterator it = m_desc_hmf.find(k);

            if (it == m_desc_hmf.end())
                return INVALID_HTTP_MSG_FIELD;

            return it->second;
        }

        void HttpMessage::set_version(const std::string &v) {
            version_ = v;
        }

        void HttpMessage::add_field(const HttpMsgField k, const std::string &v) {
            header_[k] = v;
        }

        void HttpMessage::set_body(const std::string &v) {
            body_ = v;
        }

        HttpMsgType HttpMessage::type() {
            return type_;
        }

        std::string HttpMessage::version() {
            return version_;
        }

        std::string HttpMessage::header(const HttpMsgField hmf) {
            if (header_.find(hmf) == header_.end())
                return "";

            return header_[hmf];
        }

        void HttpMessage::header(std::map<HttpMsgField, std::string> *hmf) {
            *hmf = header_;
        }

        std::string HttpMessage::body() {
            return body_;
        }

        HttpRequestMsg::HttpRequestMsg()
                : method_(INVALID_HTTP_METHOD), request_url_(""), url_(""), args_("") {
            type_ = HTTP_MSG_REQUEST;
        }

        HttpRequestMsg::~HttpRequestMsg() = default;

        void HttpRequestMsg::set_method(const HttpMethodType v) {
            method_ = v;
        }

        void HttpRequestMsg::set_request_url(const std::string &v) {
            request_url_ = v;
        }

        void HttpRequestMsg::set_url(const std::string &v) {
            url_ = v;
        }

        void HttpRequestMsg::set_args(const std::string &v) {
            args_ = v;
        }

        HttpMethodType HttpRequestMsg::method() {
            return method_;
        }

        std::string HttpRequestMsg::request_url() {
            return request_url_;
        }

        std::string HttpRequestMsg::url() {
            return url_;
        }

        std::string HttpRequestMsg::args() {
            return args_;
        }

        HttpResponseMsg::HttpResponseMsg()
                : status_(0), reason_phrase_("") {
            type_ = HTTP_MSG_RESPONSE;
        }

        HttpResponseMsg::~HttpResponseMsg() {
        }

        void HttpResponseMsg::set_status(const uint32_t v) {
            status_ = v;
        }

        void HttpResponseMsg::set_reason_phrase(const std::string &v) {
            reason_phrase_ = v;
        }

        uint32_t HttpResponseMsg::status() {
            return status_;
        }

        std::string HttpResponseMsg::reason_phrase() {
            return reason_phrase_;
        }

        HttpMsgCtx::HttpMsgCtx() {
        }

        HttpMsgCtx::~HttpMsgCtx() {
        }

        void HttpMsgCtx::parse_request_line(const std::string &line,
                                            HttpMessagePtr hm) {
            const std::string ARGS_FLAG("?");

            HttpRequestMsgPtr _hm = std::dynamic_pointer_cast<HttpRequestMsg>(hm);

            std::vector<std::string> start_line_cols;
            Split(line, &start_line_cols, " ");

            if (start_line_cols.size() < 3)
                return;

            const HttpMethodType method = hm->to_hm(start_line_cols[0]);

            if (method == INVALID_HTTP_METHOD)
                return;

            const std::string request_url(start_line_cols[1]);

            _hm->set_method(method);
            _hm->set_request_url(request_url);
            _hm->set_version(start_line_cols[2]);

            const size_t args_flag_pos = request_url.find(ARGS_FLAG);

            if (args_flag_pos == std::string::npos) {
                _hm->set_url(request_url);
            } else {
                _hm->set_url(request_url.substr(0, args_flag_pos));

                const std::string args(
                        request_url.substr(args_flag_pos + ARGS_FLAG.size()));
                _hm->set_args(hcurl::decode(args));
            }
        }

        void HttpMsgCtx::parse_status_line(const std::string &line,
                                           HttpMessagePtr hm) {
            const char SP = ' ';
            const size_t SP_SIZE = 1;

            HttpResponseMsgPtr _hm = std::dynamic_pointer_cast<HttpResponseMsg>(hm);

            // 获取版本
            const size_t version_end_pos = line.find(SP);
            if (version_end_pos == std::string::npos)
                return;
            _hm->set_version(line.substr(0, version_end_pos));

            // 获取状态码
            const size_t status_start_pos = version_end_pos + SP_SIZE;
            const size_t status_end_pos = line.find(SP, status_start_pos);
            if (status_end_pos == std::string::npos)
                return;

            const size_t status_size = status_end_pos - status_start_pos;
            const std::string status(line.substr(status_start_pos, status_size));
            _hm->set_status(stoi(status));

            // 获取原因简述
            const size_t rp_start_pos = status_end_pos + SP_SIZE;
            _hm->set_reason_phrase(line.substr(rp_start_pos));
        }

        HttpMessagePtr HttpMsgCtx::init_hm(const std::string &http) {
            const std::string FLAG("HTTP");
            const size_t FLAG_SIZE = 4;

            const std::string s(http.substr(0, FLAG_SIZE));
            const size_t pos = s.find(FLAG);

            HttpMessagePtr _hm;

            if (pos == std::string::npos)
                _hm = std::make_shared<HttpRequestMsg>();
            else
                _hm = std::make_shared<HttpResponseMsg>();

            return _hm;
        }

        void HttpMsgCtx::parse_start_line(const std::string &line, HttpMessagePtr hm) {
            if (hm->type() == HTTP_MSG_REQUEST)
                parse_request_line(line, hm);
            else
                parse_status_line(line, hm);
        }

        void HttpMsgCtx::parse_header(const std::string &header, HttpMessagePtr hm) {
            const std::string FIELD_FLAG(": ");
            const size_t FIELD_FLAG_SIZE = 2;

            std::vector<std::string> field_lines;
            Split(header, &field_lines, "\r\n");

            if (field_lines.size() < 3)
                return;

            size_t field_pos = std::string::npos;

            for (auto it : field_lines) {
                field_pos = it.find(FIELD_FLAG);

                if (field_pos == std::string::npos)
                    continue;

                const std::string name(it.substr(0, field_pos));
                const std::string value(it.substr(field_pos + FIELD_FLAG_SIZE));

                const HttpMsgField field = hm->to_hmf(name);

                if (field != INVALID_HTTP_MSG_FIELD)
                    hm->add_field(field, value);
            }
        }

        void HttpMsgCtx::parse_body(const std::string &body, HttpMessagePtr hm) {
            hm->set_body(body);
        }

        HttpMessagePtr HttpMsgCtx::parse(const std::string &http) {
            HttpMessagePtr hm;
            const std::string TWO_CRLF("\r\n\r\n");
            const size_t TWO_CRLF_SIZE = 4;

            const std::string CRLF("\r\n");
            const size_t CRLF_SIZE = 2;

            // 50 只是很随意的一个数字
            if (http.size() < 50)
                return hm;

            hm = init_hm(http);

            // 1. 获取起始行
            const size_t start_line_pos = http.find(CRLF);

            if (start_line_pos == std::string::npos)
                return hm;

            const std::string start_line(http.substr(0, start_line_pos));
            parse_start_line(start_line, hm);

            // 2. 获取消息头内容
            const size_t body_pos = http.find(TWO_CRLF);

            if (body_pos == std::string::npos)
                return hm;

            const std::string header(http.substr(start_line_pos + CRLF_SIZE, body_pos));
            parse_header(header, hm);

            // 3. 获取消息体
            parse_body(http.substr(body_pos + TWO_CRLF_SIZE), hm);

            return hm;
        }

/*根据uri，返回对应的mime type*/
        const std::string get_mime_type(const std::string &uri,
                                        const std::string &charset) {
            typedef std::map<std::string, std::string> mime_t;
            mime_t mime_types;
            mime_t::iterator it;
            const std::string _charset(charset.empty() ? "utf-8" : charset);
            mime_types["txt"] = "text/plain; charset=iso-8859-1";
            mime_types["text"] = "text/plain; charset=iso-8859-1";
            mime_types["htm"] = "text/html; charset=" + _charset;
            mime_types["html"] = "text/html; charset=" + _charset;
            mime_types["xml"] = "text/xml; charset=" + _charset;
            mime_types["jpg"] = "image/jpeg";
            mime_types["jpeg"] = "image/jpeg";
            mime_types["gif"] = "image/gif";
            mime_types["png"] = "image/png";
            mime_types["css"] = "text/css";
            mime_types["js"] = "image/javascript";
            mime_types["cgi"] = "cgi";
            mime_types["swf"] = "application/x-shockwave-flash";

            /*例如：
             * http://www.foo.com/image/logo.gif
             * 取最后的扩展名 gif */
            size_t last_dot = uri.find_last_of(".");
            const std::string ext_name = Trim(uri.substr(last_dot + 1));

            std::string mime_type("application/octet-stream");

            /*如果在map中存在key对应的值，则返回，否则返回空字符串*/
            if (mime_types.find(ext_name) != mime_types.end())
                mime_type = mime_types[ext_name];

            return mime_type;
        }

        size_t CallbackWriteFunc(void *ptr, size_t size,
                                 size_t nmemb, std::string *data) {
            data->append(reinterpret_cast<char *>(ptr), size * nmemb);
            return size * nmemb;
        }

        HttpResponseMsgPtr get_header_info(const std::string &url) {
            std::string header;
            auto curl = curl_easy_init();
            HAPPY_ASSERT(curl);

            // 响应内容不包含 HTTP 头
            curl_easy_setopt(curl, CURLOPT_HEADER, 0L);
            // 禁止检查服务器SSL证书中是否存在一个公用名(common name)
            // 禁止检查公用名是否存在，并且是否与提供的主机名匹配
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
            // 禁用服务端验证。
            curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 30L);
            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, CallbackWriteFunc);
            curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header);
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);

            const bool ret = curl_easy_perform(curl);
            curl_easy_cleanup(curl);

            if (ret == CURLE_OK) {
                HttpMsgCtx ctx;
                HttpResponseMsgPtr hm =
                        std::dynamic_pointer_cast<HttpResponseMsg>(ctx.parse(header));
                return hm;
            }

            ThrowHappyException("Cannot get http header information.");
        }

        namespace hcurl {

            uint8_t to_hex(const uint8_t &x);

            char char_to_int(char ch);

            char str_to_bin(char *str);

            uint8_t to_hex(const uint8_t &x) {
                return x > 9 ? x + 55 : x + 48;
            }

            char char_to_int(char ch) {
                if (ch >= '0' && ch <= '9')
                    return static_cast<char>(ch - '0');
                if (ch >= 'a' && ch <= 'f')
                    return static_cast<char>(ch - 'a' + 10);
                if (ch >= 'A' && ch <= 'F')
                    return static_cast<char>(ch - 'A' + 10);

                return -1;
            }

            char str_to_bin(char *str) {
                char temp_word[2];
                char chn;

                /* make the B to 11 -- 00001011 */
                temp_word[0] = char_to_int(str[0]);
                /* make the 0 to 0  -- 00000000 */
                temp_word[1] = char_to_int(str[1]);
                /* to change the BO to 10110000 */
                chn = (temp_word[0] << 4) | temp_word[1];

                return chn;
            }

            std::string encode(const std::string &s) {
                std::string val("");

                if (s.empty())
                    return val;

                uint8_t uc;

                for (size_t i = 0; i < s.size(); i++) {
                    uc = static_cast<uint8_t>(s[i]);

                    if (isalnum(uc) || uc == '-' || uc == '_' || uc == '.' || uc == '~') {
                        val.push_back(s[i]);
                    } else if (isspace(uc)) {
                        val.push_back('+');
                    } else {
                        val.push_back('%');
                        val.push_back(to_hex(uc >> 4));
                        val.push_back(to_hex(uc % 16));
                    }
                }

                return val;
            }

            std::string decode(const std::string &s) {
                std::string val("");

                if (s.empty())
                    return val;

                char buf[2];
                size_t i = 0;
                const size_t len = s.size();

                while (i < len) {
                    if (s[i] == '%') {
                        buf[0] = s[i + 1];
                        buf[1] = s[i + 2];
                        val.push_back(str_to_bin(buf));
                        i += 3;
                    } else if (s[i] == '+') {
                        val.push_back(' ');
                        ++i;
                    } else {
                        val.push_back(s[i]);
                        ++i;
                    }
                }

                return val;
            }

        } /*namespace hcurl*/

    } /* namespace hchttp */

} /* namespace happycpp */
