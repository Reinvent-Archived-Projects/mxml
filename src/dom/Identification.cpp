//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "Identification.h"

namespace mxml {
namespace dom {

static std::string EMPTY_STRING;

const std::string& Identification::creator(const std::string& type) const {
    for (const TypedValue& creator : _creators) {
        if (creator.type() == type)
            return creator.value();
    }
    return EMPTY_STRING;
}

void Identification::addCreator(const TypedValue& creator) {
    _creators.push_back(creator);
}

const std::string& Identification::rights(const std::string& type) const {
    for (const TypedValue& rights : _rights) {
        if (rights.type() == type)
            return rights.value();
    }
    return EMPTY_STRING;
}

void Identification::addRights(const TypedValue& rights) {
    _rights.push_back(rights);
}

} // namespace dom
} // namespace mxml
