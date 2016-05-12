// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
