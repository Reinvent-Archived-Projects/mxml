//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Metrics.h"


namespace mxml {

class ScrollMetrics : public Metrics {
public:
    ScrollMetrics(const dom::Score& score, const ScoreProperties& scoreProperties);

    /**
     Get the staff distance in a part.
     */
    dom::tenths_t staffDistance(std::size_t partIndex) const;

    /**
     Get the height in tenths of all staves in a part. This does not include padding above the top staff or
     below the bottom staff, therefore it is smaller than the geometry's height.
     */
    dom::tenths_t stavesHeight(std::size_t partIndex) const;

    /**
     Return the y position, in tenths, of the top line of a staff relative to the top line of the topmost staff.
     The origin of staff 1 is awlays 0.
     */
    dom::tenths_t staffOrigin(std::size_t partIndex, int staffNumber) const;

    /**
     Return the y position of a note, relative to the top line of the topmost staff.
     */
    dom::tenths_t noteY(const dom::Note& note) const;
};

} // namespace mxml
