// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>

#include <mxml/dom/OctaveShift.h>
#include <memory>

namespace mxml {
    class OctaveShiftHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::OctaveShift>> {
    public:
        void startElement(const lxml::QName& qname, const AttributeMap& attributes);

        static dom::OctaveShift::Type typeFromString(const std::string& string);

    private:
        lxml::IntegerHandler _integerHandler;
    };
}
