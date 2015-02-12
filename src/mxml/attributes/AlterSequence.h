//  Created by Alejandro Isaza on 2015-02-12.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
