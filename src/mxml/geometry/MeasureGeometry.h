//  Created by Alejandro Isaza on 2014-04-28.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"

#include <mxml/Metrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/dom/Attributes.h>
#include <mxml/dom/Measure.h>
#include <mxml/dom/Barline.h>
#include <mxml/dom/Chord.h>
#include <mxml/dom/Direction.h>
#include <mxml/dom/TimedNode.h>
#include <mxml/SpanCollection.h>

#include <memory>
#include <vector>

namespace mxml {

class MeasureGeometry : public Geometry {
public:
    static const coord_t kGraceNoteScale;

public:
    MeasureGeometry(const dom::Measure& measure,
                    const SpanCollection& spans,
                    const ScoreProperties& scoreProperties,
                    const Metrics& metrics);

    void build(bool firstMeasureInSystem);
    
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

    void adjustBounds();

private:
    void buildAttributes();
    void buildAttributes(const dom::Attributes* attributes);
    void buildBarline(const dom::Barline* barline);
    void buildTimedNode(const dom::TimedNode* barline);
    void buildChord(const dom::Chord* chord);
    void buildRest(const dom::Note* note);

    bool buildClefGeometry(const dom::Clef* clef, int staff);
    bool buildKeyGeometry(const dom::Key* key, int staff, dom::time_t time);
    bool buildTimeGeometry(const dom::Time* time, int staff);

    void centerLoneRest();
    
private:
    const dom::Measure& _measure;
    const SpanCollection& _spans;
    const ScoreProperties& _scoreProperties;
    const Metrics& _metrics;
    const std::size_t _partIndex;

    int _currentTime;

    /**
     If true this measure should show its number.
     */
    bool _showNumber;

    /**
     Track built geometires so that we know when system attributes overlap system attributes.
     */
    std::set<const dom::Node*> builtGeometries;
};

} // namespace mxml
