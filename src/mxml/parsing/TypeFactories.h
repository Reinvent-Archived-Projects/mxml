// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <lxml/RecursiveHandler.h>
#include <mxml/dom/Optional.h>
#include <mxml/dom/Types.h>

#include "GenericNode.h"


namespace mxml {
namespace parsing {

class Factory {
public:
    using AttributeMap = lxml::RecursiveHandler::AttributeMap;
    using Iterator = AttributeMap::const_iterator;

public:
    static bool yesNoValue(const std::string& string);
    static dom::tenths_t tenthsValue(const std::string& string);
};

class JustifyFactory : public Factory {
public:
    static dom::Optional<dom::Justify> buildFromAttributes(const AttributeMap& attributes);
    static dom::Optional<dom::Justify> buildFromGenericNode(const GenericNode& node);

protected:
    static dom::Optional<dom::Justify> value(const std::string& string);
};

class HAlignFactory : public Factory {
public:
    static dom::Optional<dom::HAlign> buildFromAttributes(const AttributeMap& attributes);
    static dom::Optional<dom::HAlign> buildFromGenericNode(const GenericNode& node);

protected:
    static dom::Optional<dom::HAlign> value(const std::string& string);
};

class VAlignFactory : public Factory {
public:
    static dom::Optional<dom::VAlign> buildFromAttributes(const AttributeMap& attributes);
    static dom::Optional<dom::VAlign> buildFromGenericNode(const GenericNode& node);

protected:
    static dom::Optional<dom::VAlign> value(const std::string& string);
};

class PrintObjectFactory : public Factory {
public:
    static bool buildFromAttributes(const AttributeMap& attributes);
    static bool buildFromGenericNode(const GenericNode& node);
};

} // namespace parsing
} // namespace mxml
