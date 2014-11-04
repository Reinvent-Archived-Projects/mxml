//  Created by Alejandro Isaza on 2014-07-09.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "SyllabicHandler.h"
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

void SyllabicHandler::endElement(const lxml::QName& qname, const std::string& contents) {
    _result.setType(typeFromString(contents));
}

dom::Syllabic::Type SyllabicHandler::typeFromString(const std::string& string) {
    if (string == "single")
        return dom::Syllabic::Single;
    else if (string == "begin")
        return dom::Syllabic::Begin;
    else if (string == "end")
        return dom::Syllabic::End;
    else if (string == "middle")
        return dom::Syllabic::Middle;
    throw dom::InvalidDataError("Invalid syllabic type " + string);
}

} // namespace
