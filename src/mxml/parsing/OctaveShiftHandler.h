//  Created by Alejandro Isaza on 2015-01-19.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>

#include <mxml/dom/OctaveShift.h>
#include <memory>

namespace mxml {
    class OctaveShiftHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::OctaveShift>> {
    public:
        void startElement(const lxml::QName& qname, const AttributeMap& attributes);\

        static dom::OctaveShift::Type typeFromString(const std::string& string);

    private:
        lxml::IntegerHandler _integerHandler;
    };
}
