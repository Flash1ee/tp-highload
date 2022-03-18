//
// Created by Dmitriy Varin on 18.03.2022.
//

#pragma once
#include <string>

const std::string
    RESPONSE_OK = "HTTP/1.1 200 OK\r\n"
                  "Connection: close\r\n"
                  "Server: Flash1ee/1.0 (Ubuntu)\r\n"
                  "Date: ";
const std::string
    NOT_FOUND = "HTTP/1.1 404 Not Found\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n"
                "<html><body><h1>404 Not Found</h1></body></html>";
const std::string
    NOT_IMPLEMENTED = "HTTP/1.1 405 Method Not Implemented\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n";

const std::string
    VERSION_NOT_SUPPORTED =
    "HTTP/1.1 505 HTTP Version Not Supported\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n";

const std::string
    FORBIDDEN = "HTTP/1.1 403 Forbidden\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n";
const std::string
    FORBIDDEN_BODY = "<html><body><h1>403 Forbidden</h1></body></html>";