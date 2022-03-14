//
// Created by Dmitriy Varin on 14.03.2022.
//
#include <sstream>
#include <chrono>
#include <fstream>
#include <iostream>

#include "http_handler.h++"

std::string HTTPHandler::handle(const std::string &request) const{
    // Parsing
    std::stringstream reqStream(request);
    std::string line;
    std::getline(reqStream, line);

    std::stringstream requestLine(line);
    std::string method;
    std::getline(requestLine, method, ' ');

    if(method == "GET") {
        std::string url;
        std::getline(requestLine, url, ' ');

        std::string version;
        std::getline(requestLine, version, ' ');
        if(version != "HTTP/1.1\r") { // if the version is not 1.1
            std::cout << version << "\n";
            std::string response = "HTTP/1.1 505 HTTP Version Not Supported\r\nConnection: close\r\n";
            return response;
        }

        url.insert(0, ".."); // httptest will contain all the files the server serves
//        if(url == "../httptest/") { // if the client doesn't specify a file we send the default one
//            url += "index.html";
//        }
        if(url == "..") { // if the client doesn't specify a file we send the default one
            url += "index.html";
        }

        // Loading the requested file
        std::fstream targetFile;
        targetFile.open(url, std::ios::in);
        if(!targetFile.is_open()) { // if it doesn't open we handle the error
            std::cerr << "Failed to load the file\n";
            // for now we assume that every time a file doesn't open, it's a bad request
            std::string response = "HTTP/1.1 404 Not Found\r\nConnection: close\r\n\r\n";
            response += "<html><body><h1>404 Not Found</h1></body></html>";
            return response;
        }
        // If everything is good we load the file
        std::string fileLine;
        std::string fileContent = "";
        while (std::getline(targetFile, fileLine)) {
            fileContent += "\n" + fileLine;
        }
        targetFile.close();

        // Preparing and sending the response
        std::string response = _head();
        auto length = fileContent.size();
        response = response + "Content-Length: " + std::to_string(length);
        response += "\r\n" + fileContent;

        return response;


    } else if(method == "HEAD") {
        return _head();
    } else {
        return _notImplimented();
    }
}

std::string HTTPHandler::_notImplimented() const{
    std::string response = "HTTP/1.1 501 Not Implemented\r\nConnection: close\r\n\r\n";
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
