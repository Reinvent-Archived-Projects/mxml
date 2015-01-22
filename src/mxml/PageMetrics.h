//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Metrics.h"


namespace mxml {

class PageMetrics : public Metrics {
public:
    PageMetrics(const dom::Score& score, const ScoreProperties& scoreProperties);

    /**
     Get the vertical distance between the system at the given index and the previous system.
     */
    dom::tenths_t systemDistance(std::size_t systemIndex) const;

    /**
     Get the distance between the first and second staves for the given system and part.
     */
    dom::tenths_t staffDistance(std::size_t systemIndex, std::size_t partIndex) const;

    /**
     Get the total height in tenths of all staves in a part. This does not include padding above the top staff or below
     the bottom staff, therefore it is smaller than the geometry's height.
     */
    dom::tenths_t stavesHeight(std::size_t systemIndex, std::size_t partIndex) const;

    /**
     Return the y position, in tenths, of the top line of a staff relative to the top line of the topmost staff. The
     origin of staff 1 is awlays 0.
     */
    dom::tenths_t staffOrigin(std::size_t systemIndex, std::size_t partIndex, int staffNumber) const;

    /**
     Return the y position of a note, relative to the top line of the topmost staff.
     */
    dom::tenths_t noteY(const dom::Note& note) const;
};
    
} // namespace mxml
