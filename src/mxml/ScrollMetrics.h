//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
