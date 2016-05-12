// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "AttributeSequence.h"


namespace mxml {

class AlterSequence : public AttributeSequence<int> {
public:
    /**
     Add an alter from an instance of the note node.
     */
    void addFromNote(std::size_t partIndex, std::size_t measureIndex, const dom::Note& note);

    /**
     Get active alter
     */
    int find(const Index& index, int defaultAlter) const;
};

} // namespace mxml
