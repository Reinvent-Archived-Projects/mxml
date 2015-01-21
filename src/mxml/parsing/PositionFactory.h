//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include "GenericNode.h"
#include "TypeFactories.h"
#include <mxml/dom/Position.h>


namespace mxml {
namespace parsing {

class PositionFactory : public Factory {
public:
    static dom::Position buildFromAttributes(const AttributeMap& attributes);
    static dom::Position buildFromGenericNode(const GenericNode& node);

protected:
    static dom::Optional<dom::tenths_t> value(const std::string& string);
};

} // namespace parsing
} // namespace mxml
