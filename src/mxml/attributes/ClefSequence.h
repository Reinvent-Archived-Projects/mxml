// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "AttributeSequence.h"
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Clef.h>
#include <mxml/dom/Types.h>


namespace mxml {

class ClefSequence : public AttributeSequence<const dom::Clef*> {
public:
    /**
     Add all the clefs from an instance of the attributes node.
     */
    void addFromAttributes(std::size_t partIndex, std::size_t measureIndex, const dom::Attributes& attributes);

    /**
     Get active key for the given part, measure, staff and time.
     */
    const dom::Clef* find(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const;
    
private:
    std::vector<int> _staves;
};

} // namespace mxml
