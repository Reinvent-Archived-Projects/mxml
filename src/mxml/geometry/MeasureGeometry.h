//  Created by Alejandro Isaza on 2014-04-28.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"

#include <mxml/AttributesManager.h>
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

class PartGeometry;

class MeasureGeometry : public Geometry {
public:
    static const coord_t kGraceNoteScale;
    static const coord_t kVerticalPadding;

public:
    MeasureGeometry(const dom::Measure& measure,
                    const PartGeometry& partGeometry,
                    const SpanCollection& spans,
                    AttributesManager& attributesManager);
    
    const dom::Measure& measure() const {
        return _measure;
    }
    const PartGeometry& partGeometry() const {
        return _partGeometry;
    }
    
    const SpanCollection& spans() const {
        return _spans;
    }

    bool naturalSpacing() const {
        return _spans.naturalSpacing();
    }
    
    /**
     */
    void build();
    
private:
    void buildAttributes(const dom::Attributes* attributes);
    void buildBarline(const dom::Barline* barline);
    void buildTimedNode(const dom::TimedNode* barline);
    void buildChord(const dom::Chord* chord);
    void buildRest(const dom::Note* note);

    void centerLoneRest();
    
private:
    const dom::Measure& _measure;
    const PartGeometry& _partGeometry;
    const SpanCollection& _spans;
    AttributesManager& _attributesManager;
    
    std::size_t _measureIndex;
    
    int _currentTime;
};

} // namespace mxml
