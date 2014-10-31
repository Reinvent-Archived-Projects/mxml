//  Created by Alejandro Isaza on 2014-07-09.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "StringHandler.h"
#include "SyllabicHandler.h"

#include "dom/Lyric.h"

namespace mxml {

class LyricHandler : public lxml::BaseRecursiveHandler<dom::Lyric> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    lxml::StringHandler _stringHandler;
    SyllabicHandler _syllabicHandler;
};

} // namespace
