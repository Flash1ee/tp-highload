//
// Created by Dmitriy Varin on 14.03.2022.
//
#include <sstream>
#include <chrono>
#include <fstream>
#include <iostream>
#include <vector>
#include <algorithm>

#include "http_handler.h++"

std::string decode_url(const std::string& url) {
    std::string decoded_url;
    for (int i = 0; i < url.size(); i++) {
        if (url[i] == '%') {
            decoded_url += static_cast<char>(strtoll(url.substr(i + 1, 2).c_str(), nullptr, 16));
            i += 2;
        } else {
            decoded_url += url[i];
        }
    }
    return decoded_url;
}
std::string read_file(const std::string& path) {
    std::ifstream file(path,std::ios::in | std::ios::binary);
    std::stringstream fl;
    if (file.is_open()) {
        fl << file.rdbuf();
    }

    file.close();
    return fl.str();
}

std::vector<std::string> tokenize(const std::string& s, const std::string& del = " ")
{
    std::vector<std::string> res;
    int start = 0;
    int end = s.find(del);
    while (end != -1) {
        res.push_back(s.substr(start, end - start));
        start = end + del.size();
        end = s.find(del, start);
    }
    res.push_back(s.substr(start, end - start));
    return res;
}
std::string strip_query_params(const std::string &s) {
    auto tokens = tokenize(s, "/");
    auto last = tokens[tokens.size()-1];
    auto checkParams = last.rfind('?');
    if (checkParams != -1) {
        last = last.substr(0, checkParams);
    }
    std::string newPath;
    for (auto i = 0; i < tokens.size() - 1; i++) {
        newPath += tokens[i] + "/";
    }
    newPath += last;

    return newPath;
}
std::string HTTPHandler::getResponseNoBody(std::stringstream& request, std::string& url) const {
    std::string version;
    std::getline(request, version, ' ');
    if(version != "HTTP/1.1\r" && version != "HTTP/1.0\r") { // if the version is not 1.1
        std::cout << version << "\n";
        std::string response = "HTTP/1.1 505 HTTP Version Not Supported\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n";
        return response;
    }
    url = decode_url(url);
    url = strip_query_params(url);
    url.insert(0, ".."); // httptest will contain all the files the server serves
    bool is_dir = false;
    if (std::filesystem::is_directory(url)) {
        url += "index.html";
        is_dir = true;
    }
    // Loading the requested file
    std::fstream targetFile;
    targetFile.open(url, std::ios::in);
    if(!targetFile.is_open()) {// if it doesn't open we handle the error
        if (is_dir) {
            std::cerr << "Failed to load index.html in dir path\n";
            std::string response = "HTTP/1.1 403 Forbidden\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n";
            return response;
        }
        std::cerr << "Failed to load the file\n";
        // for now we assume that every time a file doesn't open, it's a bad request

        return _notFound();
    }
    // If everything is good we load the file
    char fileLine[2048];
    std::string fileContent;
    while (targetFile.read(fileLine, 1024)) {
        fileContent += fileLine;
    }
    fileContent.append(fileLine, targetFile.gcount());
    targetFile.close();

    // Preparing and sending the response
    std::string response = _head();
    auto length = fileContent.size();
    response = response + "Content-Length: " + std::to_string(length);

    response += "\r\n\r\n";

    std::cout << response;

    return response;
}
std::string HTTPHandler::handle(const std::string &request) const{
    // Parsing
    std::stringstream reqStream(request);
    std::string line;
    std::getline(reqStream, line);

    std::stringstream requestLine(line);
    std::string method;
    std::getline(requestLine, method, ' ');

    std::string url;

    std::getline(requestLine, url, ' ');

    if (url.find("../") != std::string::npos) {
        return _notFound();
    }

    if(method == "GET") {
        std::string version;
        std::getline(requestLine, version, ' ');
        if(version != "HTTP/1.1\r") { // if the version is not 1.1
            std::cout << version << "\n";
            std::string response = "HTTP/1.1 505 HTTP Version Not Supported\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n";
            return response;
        }
        url = decode_url(url);
        url = strip_query_params(url);
        url.insert(0, ".."); // httptest will contain all the files the server serves
//        if(url == "../httptest/") { // if the client doesn't specify a file we send the default one
//            url += "index.html";
//        }
//        auto splitted = tokenize(url, "/");
        bool is_dir = false;
        if (std::filesystem::is_directory(url)) {
//        if(url == "..") { // if the client doesn't specify a file we send the default one
            url += "index.html";
            is_dir = true;
        }
//        std::string::replace(url.begin(), url.end(), '%20', ' ');

        // Loading the requested file
        std::fstream targetFile;
        targetFile.open(url, std::ios::in);
        if(!targetFile.is_open()) {// if it doesn't open we handle the error


            if (is_dir) {
                std::cerr << "Failed to load index.html in dir path\n";
                std::string response = "HTTP/1.1 403 Forbidden\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n";
                response += "<html><body><h1>403 Forbidden</h1></body></html>";
                return response;
            }
            std::cerr << "Failed to load the file\n";
            // for now we assume that every time a file doesn't open, it's a bad request

            return _notFound();
        }
        // If everything is good we load the file
        auto res = read_file(url);
//        while (std::getline(targetFile, fileLine, '\n')) {
//            fileContent += fileLine;
//        }
//        fileContent = fileContent.substr(0, fileContent.size() - 1);
//        fileContent.append(fileLine, targetFile.gcount());

        // Preparing and sending the response
        std::string response = _head();
        auto length = res.size();
        response = response + "Content-Length: " + std::to_string(length)  + "\r\n";;
        if (std::filesystem::is_regular_file(url)) {
            response += "Content-Type: " + parse_mime(url);
        }
        response += "\r\n\r\n" + res;
        return response;


    } else if(method == "HEAD") {
        return getResponseNoBody(requestLine, url);
    } else {
        return _notImplimented();
    }
}

std::string HTTPHandler::_notImplimented() const{
    std::string response = "HTTP/1.1 405 Method Not Implemented\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n";
    return response;
}
std::string HTTPHandler::_notFound() const{
    std::string response =  "HTTP/1.1 404 Not Found\r\nServer: Flash1ee/1.0\r\nConnection: close\r\n\r\n";
    response += "<html><body><h1>404 Not Found</h1></body></html>";

    return response;
}

std::string HTTPHandler::_head() const{
    std::string response = "HTTP/1.1 200 OK\r\n\
Connection: close\r\n\
Server: Flash1ee/1.0 (Ubuntu)\r\n\
Date: ";
    response += _getDate() + "\r\n";
    return response;
}

std::string HTTPHandler::_getDate() const{
    std::time_t rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string date = ctime(&rawTime);
    std::string formatedDate = date.substr(0, 3) + ", " + date.substr(8, 3) + date.substr(4, 4) + date.substr(20, 4)
                               + " " + date.substr(11, 8);
    return formatedDate;
}
std::string HTTPHandler::parse_mime(const std::string& s) const {
    auto xs = s.substr(s.rfind('.'), s.size());
    if (xs == ".html") {
        return "text/html";
    } else if (xs == ".js") {
        return "application/javascript";
    } else if (xs == ".css") {
        return "text/css";
    } else if (xs == ".jpg") {
        return "image/jpeg";
    } else if (xs == ".jpeg") {
        return "image/jpeg";
    } else if (xs == ".png") {
        return "image/png";
    } else if (xs == ".gif") {
        return "image/gif";
    } else if (xs == ".swf") {
        return "application/x-shockwave-flash";
    }

    return "none";
}