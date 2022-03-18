//
// Created by Dmitriy Varin on 18.03.2022.
//
#include <vector>
#include <fstream>
#include <sstream>
#include "utils.h++"

std::string notImplimented() {
    std::string response = NOT_IMPLEMENTED;
    return response;
}
std::string notFound() {
    std::string response = NOT_FOUND;
    return response;
}

std::string head() {
    std::string response = RESPONSE_OK;
    response += getDate() + "\r\n";
    return response;
}

std::string getDate() {
    std::time_t rawTime = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::string date = ctime(&rawTime);
    std::string formatedDate = date.substr(0, 3) + ", " + date.substr(8, 3) + date.substr(4, 4) + date.substr(20, 4)
        + " " + date.substr(11, 8);
    return formatedDate;
}

std::string decode_url(const std::string &url) {
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
std::string read_file(const std::string &path) {
    std::ifstream file(path, std::ios::in | std::ios::binary);
    std::stringstream fl;
    if (file.is_open()) {
        fl << file.rdbuf();
    }

    file.close();
    return fl.str();
}

std::vector<std::string> tokenize(const std::string &s, const std::string &del = " ") {
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
    auto last = tokens[tokens.size() - 1];
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

std::string parse_mime(const std::string &s) {
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