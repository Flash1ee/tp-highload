//
// Created by Dmitriy Varin on 14.03.2022.
//

#pragma once

#include <string>

class HTTPHandler {
 public:
  std::string handle(const std::string &request) const;
};
