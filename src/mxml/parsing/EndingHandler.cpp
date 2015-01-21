//  Created by Alejandro Isaza on 2014-05-01.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "EndingHandler.h"
#include <lxml/IntegerHandler.h>
#include <mxml/dom/InvalidDataError.h>

namespace mxml {

using dom::Ending;
using lxml::QName;

static const char* kNumberAttribute = "number";
static const char* kTypeAttribute = "type";

void EndingHandler::startElement(const QName& qname, const AttributeMap& attributes) {
    _result.reset(new Ending());
    
    auto type = attributes.find(kTypeAttribute);
    if (type != attributes.end())
        _result->setType(typeFromString(type->second));
    
    auto number = attributes.find(kNumberAttribute);
    if (number != attributes.end())
        _result->setNumbers(parseNumberList(number->second));
}

void EndingHandler::endElement(const QName& qname, const std::string& contents) {
    _result->setContent(contents);
}

Ending::Type EndingHandler::typeFromString(const std::string& string) {
    if (string == "start")
        return Ending::kStart;
    else if (string == "stop")
        return Ending::kStop;
    else if (string == "discontinue")
        return Ending::kDiscontinue;
    throw dom::InvalidDataError("Invalid ending type " + string);
}

std::set<int> EndingHandler::parseNumberList(const std::string& string) {
    std::set<int> set;
    
    std::size_t begin = 0;
    std::size_t end = string.find(',');
    while (begin < string.length()) {
        std::size_t n;
        if (end > string.length())
            n = string.length() - begin;
        else
            n = end - begin;
        
        set.insert(lxml::IntegerHandler::parseInteger(string.substr(begin, n)));
        begin = end;
        if (end < string.length())
            end = string.find(',', end + 1);
    }
    return set;
}

} // namespace mxml
