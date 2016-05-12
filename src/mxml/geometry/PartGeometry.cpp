// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "PartGeometry.h"
#include "TieGeometry.h"
#include "PlacementGeometry.h"


namespace mxml {

PartGeometry::PartGeometry(const dom::Part& part, const ScoreProperties& scoreProperties, const Metrics& metrics)
: _part(part),
  _scoreProperties(scoreProperties),
  _metrics(metrics)
{    _staffDistance = metrics.staffDistance();
}

dom::tenths_t PartGeometry::noteY(const dom::Note& note) const {
    return staffOrigin(note.staff()) + _metrics.staffY(note);
}

void PartGeometry::setActiveRange(std::size_t startMeasureIndex, std::size_t endMeasureIndex) {
    if (_measureGeometries.empty())
        return;

    const auto partStartMeasureIndex = _measureGeometries.front()->measure().index();
    const auto partEndMeasureIndex = _measureGeometries.back()->measure().index() + 1;

    if (startMeasureIndex <= partStartMeasureIndex && endMeasureIndex >= partEndMeasureIndex) {
        setActive(true);
    } else if (startMeasureIndex >= partEndMeasureIndex || endMeasureIndex <= partStartMeasureIndex) {
        setActive(false);
    } else {
        setActive(true);

        mxml::coord_t start = size().width;
        mxml::coord_t end = 0;
        for (auto& measureGeometry : _measureGeometries) {
            auto index = measureGeometry->measure().index();
            auto active = index >= startMeasureIndex && index < endMeasureIndex;
            if (active) {
                start = std::min(start, measureGeometry->frame().min().x);
                end = std::max(end, measureGeometry->frame().max().x);
            }
            measureGeometry->setActive(active);
        }
        for (auto& tieGeometry : _tieGeometries) {
            auto frame = tieGeometry->frame();
            tieGeometry->setActive(frame.max().x >= start && frame.min().x <= end);
        }
        for (auto& directionGeometry : _directionGeometries) {
            auto frame = directionGeometry->frame();
            directionGeometry->setActive(frame.max().x >= start && frame.min().x <= end);
        }
    }
}
    
} // namespace mxml
