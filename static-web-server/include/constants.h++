//
// Created by Dmitry Varin on 27.02.2022.
//
#pragma once

const int POOL_SIZE = 50;
const int PORT = 8080;
const int CNT_CONN = 400;
const int REQ_SIZE = 1024;
//
const int read_size = 1024;
const std::string HEAD = "HEAD";
const std::string GET = "GET";
const std::string ROOT_FILE = "index.html";
const std::string CONTENT_LENGTH = "Content-Length:";
const std::string CONTENT_TYPE = "Content-Type:";
const std::string HTTP_1_1 = "HTTP/1.1\r";
const std::string HTTP_1_0 = "HTTP/1.0\r";