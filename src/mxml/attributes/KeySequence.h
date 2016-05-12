// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "AttributeSequence.h"
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Key.h>
#include <mxml/dom/Types.h>


namespace mxml {

class KeySequence : public AttributeSequence<const dom::Key*> {
public:
    /**
     Add all the keys from an instance of the attributes node.
     */
    void addFromAttributes(std::size_t partIndex, std::size_t measureIndex, const dom::Attributes& attributes);

    /**
     Get active key for the given part, measure, staff and time.
     */
    const dom::Key* find(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const;

private:
    std::vector<int> _staves;
};

} // namespace mxml
