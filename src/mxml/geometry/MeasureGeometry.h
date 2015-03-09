//  Created by Alejandro Isaza on 2014-04-28.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"

#include <mxml/Metrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/SpanCollection.h>
#include <mxml/dom/Measure.h>


namespace mxml {

class MeasureGeometry : public Geometry {
public:
    static const coord_t kGraceNoteScale;

public:
    MeasureGeometry(const dom::Measure& measure,
                    const SpanCollection& spans,
                    const ScoreProperties& scoreProperties,
                    const Metrics& metrics);
    
    const dom::Measure& measure() const {
        return _measure;
    }
    
    const SpanCollection& spans() const {
        return _spans;
    }

    const Metrics& metrics() const {
        return _metrics;
    }
    
    bool naturalSpacing() const {
        return _spans.naturalSpacing();
    }

    bool showNumber() const {
        return _showNumber;
    }
    
private:
    const dom::Measure& _measure;
    const SpanCollection& _spans;
    const ScoreProperties& _scoreProperties;
    const Metrics& _metrics;
    const std::size_t _partIndex;

    /**
     If true this measure should show its number.
     */
    bool _showNumber;

    friend class MeasureGeometryFactory;
};

} // namespace mxml
