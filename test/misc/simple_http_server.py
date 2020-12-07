#!/usr/bin/python3

import socketserver
from http.server import BaseHTTPRequestHandler


class ServerHandler(BaseHTTPRequestHandler):
    def do_HEAD(self):
        self.send_response(200)
        self.send_header('Content-Type', 'text/plain')
        self.send_header('Content-Length', '100')
        self.send_header('Last-Modified', 'Thu, 28 Jul 2016 05:37:47 GMT')
        self.send_header('Connection', 'keep-alive')
        self.send_header('ETag', '"57999a2b-2"')
        self.send_header('Accept-Ranges', 'bytes')
        self.end_headers()


Handler = ServerHandler

httpd = socketserver.TCPServer(("127.0.0.1", 8887), Handler)

httpd.serve_forever()
