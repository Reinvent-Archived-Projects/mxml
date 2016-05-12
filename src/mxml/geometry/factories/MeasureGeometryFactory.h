// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/Metrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/SpanCollection.h>
#include <mxml/dom/Barline.h>
#include <mxml/dom/Measure.h>
#include <mxml/dom/Tuplet.h>
#include <mxml/geometry/MeasureGeometry.h>

#include <memory>
#include <vector>

#include "ChordGeometryFactory.h"


namespace mxml {

class ChordGeometry;

class MeasureGeometryFactory {
public:
    MeasureGeometryFactory(const SpanCollection& spans,
                           const ScoreProperties& scoreProperties,
                           const Metrics& metrics);

    std::unique_ptr<MeasureGeometry> build(const dom::Measure& measure, bool firstMeasureInSystem);

    void adjustBounds(MeasureGeometry* geometry);

private:
    void buildAttributes();
    void buildAttributes(const dom::Attributes* attributes);
    void buildBarline(const dom::Barline* barline);
    void buildTimedNode(const dom::TimedNode* barline);
    void buildChord(const dom::Chord* chord);
    void placeChord(ChordGeometry* chordGeom);
    void buildRest(const dom::Note* note);

    bool buildClefGeometry(const dom::Clef* clef, int staff);
    bool buildKeyGeometry(const dom::Key* key, int staff, dom::time_t time);
    bool buildTimeGeometry(const dom::Time* time, int staff);
    void buildBeams();

    void buildTuplet(const ChordGeometry* chordGeometry, const dom::Tuplet* tuplet);
    
    void centerWholeRests();
    
private:
    const SpanCollection& _spans;
    const ScoreProperties& _scoreProperties;
    const Metrics& _metrics;
    const std::size_t _partIndex;

    std::size_t _measureIndex;
    std::unique_ptr<MeasureGeometry> _geometry;

    ChordGeometryFactory _chordGeometryFactory;

    int _currentTime;

    /**
     Track built geometires so that we know when system attributes overlap system attributes.
     */
    std::set<const dom::Node*> builtGeometries;

    std::map<int, std::pair<const ChordGeometry*, const dom::Tuplet*>> _tupletStart;
};

} // namespace mxml
