// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
