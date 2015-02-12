//  Created by Alejandro Isaza on 2015-02-12.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
