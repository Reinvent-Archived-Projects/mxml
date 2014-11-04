//  Created by Alejandro Isaza on 2/17/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include <stdexcept>
#include <string>

namespace mxml {
namespace dom {

class InvalidDataError : public std::runtime_error {
public:
    explicit InvalidDataError(const std::string& s) : std::runtime_error(s) {}
    explicit InvalidDataError(const char* s) : std::runtime_error(s) {}
};

} // namespace dom
} // namespace mxml
