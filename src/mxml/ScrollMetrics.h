// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Metrics.h"


namespace mxml {

class ScrollMetrics : public Metrics {
public:
    ScrollMetrics(const dom::Score& score, const ScoreProperties& scoreProperties, std::size_t partIndex);

    /**
     Get the staff distance in a part.
     */
    dom::tenths_t staffDistance() const;
};

} // namespace mxml
