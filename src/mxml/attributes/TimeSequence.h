//  Created by Alejandro Isaza on 2015-02-12.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include "AttributeSequence.h"
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Time.h>
#include <mxml/dom/Types.h>


namespace mxml {

class TimeSequence : public AttributeSequence<const dom::Time*> {
public:
    /**
     Add all the keys from an instance of the attributes node.
     */
    void addFromAttributes(std::size_t measureIndex, const dom::Attributes& attributes);

    /**
     Get active key for the given part, measure, staff and time.
     */
    const dom::Time* find(std::size_t measureIndex) const;
    
private:
    static const dom::Time _defaultTime;
};
    
} // namespace mxml
