// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "SyllabicHandler.h"
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using dom::Syllabic;

void SyllabicHandler::startElement(const lxml::QName& qname, const AttributeMap& attributes) {
    _result.reset(new Syllabic());
}

void SyllabicHandler::endElement(const lxml::QName& qname, const std::string& contents) {
    _result->setType(typeFromString(contents));
}

dom::Syllabic::Type SyllabicHandler::typeFromString(const std::string& string) {
    if (string == "single")
        return dom::Syllabic::Type::Single;
    else if (string == "begin")
        return dom::Syllabic::Type::Begin;
    else if (string == "end")
        return dom::Syllabic::Type::End;
    else if (string == "middle")
        return dom::Syllabic::Type::Middle;
    throw dom::InvalidDataError("Invalid syllabic type " + string);
}

} // namespace
