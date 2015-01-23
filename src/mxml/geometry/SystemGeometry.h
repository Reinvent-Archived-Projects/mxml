//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/PageMetrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/dom/Part.h>

#include "Geometry.h"
#include "MeasureGeometry.h"
#include "TieGeometry.h"


namespace mxml {

class SystemGeometry : public Geometry {
public:
    SystemGeometry(const dom::Part& part, const System& system, const ScoreProperties& scoreProperties, const PageMetrics& metrics);

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

    dom::tenths_t staffDistance() const {
        return _staffDistance;
    }
    
private:
    const dom::Part& _part;
    const System& _system;
    const ScoreProperties& _scoreProperties;
    const PageMetrics& _metrics;

    dom::tenths_t _staffDistance;

    std::vector<MeasureGeometry*> _measureGeometries;
    std::vector<TieGeometry*> _tieGeometries;
    std::vector<PlacementGeometry*> _directionGeometries;

    friend class SystemGeometryFactory;
};

} // namespace mxml
