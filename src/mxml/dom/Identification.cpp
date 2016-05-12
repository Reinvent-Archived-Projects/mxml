// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "Identification.h"

namespace mxml {
namespace dom {

static std::string EMPTY_STRING;

const std::string& Identification::creator(const std::string& type) const {
    for (const auto& creator : _creators) {
        if (creator->type() == type)
            return creator->value();
    }
    return EMPTY_STRING;
}

void Identification::addCreator(std::unique_ptr<TypedValue> creator) {
    _creators.push_back(std::move(creator));
}

const std::string& Identification::rights(const std::string& type) const {
    for (const auto& rights : _rights) {
        if (rights->type() == type)
            return rights->value();
    }
    return EMPTY_STRING;
}

void Identification::addRights(std::unique_ptr<TypedValue> rights) {
    _rights.push_back(std::move(rights));
}

} // namespace dom
} // namespace mxml
