// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "EndingHandler.h"
#include "RepeatHandler.h"
#include <lxml/StringHandler.h>

#include <mxml/dom/Barline.h>
#include <memory>

namespace mxml {

class BarlineHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Barline>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
    static dom::Barline::Style styleFromString(const std::string& string);
    static dom::Barline::Location locationFromString(const std::string& string);
    
private:
    lxml::StringHandler _stringHandler;
    EndingHandler _endingHandler;
    RepeatHandler _repeatHandler;
};

} // namespace mxml
