// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
