//
// Created by Dmitriy Varin on 18.03.2022.
//
#pragma once

#include <string>
#include "responses.h++"

std::string decode_url(const std::string &url);
std::string read_file(const std::string &path);
std::vector<std::string> tokenize(const std::string &s, const std::string &del);
std::string strip_query_params(const std::string &s);
//
std::string parse_mime(const std::string &s);
std::string notImplimented();
std::string notFound();

std::string head();
std::string getDate();
