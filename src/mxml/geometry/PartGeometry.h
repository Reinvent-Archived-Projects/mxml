// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Geometry.h"
#include "MeasureGeometry.h"

#include <mxml/ScoreProperties.h>
#include <mxml/Metrics.h>
#include <mxml/dom/Part.h>


namespace mxml {

class BarlineGeometry;
class ChordGeometry;
class TieGeometry;
class PlacementGeometry;

class PartGeometry : public Geometry {
public:
    explicit PartGeometry(const dom::Part& part, const ScoreProperties& scoreProperties, const Metrics& metrics);
    
    const dom::Part& part() const {
        return _part;
    }
    
    const std::vector<MeasureGeometry*>& measureGeometries() const {
        return _measureGeometries;
    }
    const std::vector<TieGeometry*>& tieGeometries() const {
        return _tieGeometries;
    }
    const std::vector<PlacementGeometry*>& directionGeometries() const {
        return _directionGeometries;
    }
    std::vector<PlacementGeometry*>& directionGeometries() {
        return _directionGeometries;
    }

    std::size_t staves() const {
        return _scoreProperties.staves(_part.index());
    }
    dom::tenths_t staffDistance() const {
        return _staffDistance;
    }
    dom::tenths_t stavesHeight() const {
        return Metrics::stavesHeight(staves(), _staffDistance);
    }
    dom::tenths_t staffOrigin(int staffNumber) const {
        return (staffNumber - 1) * (Metrics::staffHeight() + _staffDistance);
    }

    /**
     Return the y position of a note in this part geometry.
     */
    dom::tenths_t noteY(const dom::Note& note) const;

    void setActiveRange(std::size_t startMeasureIndex, std::size_t endMeasureIndex);

private:
    const dom::Part& _part;
    const ScoreProperties& _scoreProperties;
    const Metrics& _metrics;
    
    dom::tenths_t _staffDistance;

    std::vector<MeasureGeometry*> _measureGeometries;
    std::vector<TieGeometry*> _tieGeometries;
    std::vector<PlacementGeometry*> _directionGeometries;
    
    friend class PartGeometryFactory;
};

} // namespace mxml
