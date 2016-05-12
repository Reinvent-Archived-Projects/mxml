// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Metrics.h"


namespace mxml {

class PageMetrics : public Metrics {
public:
    PageMetrics(const dom::Score& score, const ScoreProperties& scoreProperties, std::size_t systemIndex, std::size_t partIndex);

    std::size_t systemIndex() const {
        return _systemIndex;
    }
    
    /**
     Get the vertical distance between the system and the previous system.
     */
    dom::tenths_t systemDistance() const;

    /**
     Get the distance between the first and second staves.
     */
    dom::tenths_t staffDistance() const;

private:
    std::size_t _systemIndex;
};
    
} // namespace mxml
