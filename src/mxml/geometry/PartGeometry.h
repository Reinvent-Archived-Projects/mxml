//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include "MeasureGeometry.h"
#include "TieGeometry.h"

#include <mxml/AttributesManager.h>
#include <mxml/dom/Part.h>
#include <mxml/dom/Direction.h>
#include <mxml/dom/Ornaments.h>

namespace mxml {

class BarlineGeometry;
class ChordGeometry;

class PartGeometry : public Geometry {
public:
    explicit PartGeometry(const dom::Part& part, const SpanCollection& spans);
    
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
    
private:
    const dom::Part& _part;
    const SpanCollection& _spans;
    
    std::vector<MeasureGeometry*> _measureGeometries;
    std::vector<TieGeometry*> _tieGeometries;
    std::vector<PlacementGeometry*> _directionGeometries;
    
    friend class PartGeometryFactory;
};

} // namespace mxml
