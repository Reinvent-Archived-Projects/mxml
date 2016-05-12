// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "AttributeSequence.h"
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Types.h>


namespace mxml {

class DivisionsSequence : public AttributeSequence<int> {
public:
    /**
     Add all the keys from an instance of the attributes node.
     */
    void addFromAttributes(std::size_t measureIndex, const dom::Attributes& attributes);

    /**
     Get active key for the given part, measure, staff and time.
     */
    int find(std::size_t measureIndex) const;
};
    
} // namespace mxml
