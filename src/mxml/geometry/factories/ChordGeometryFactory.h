//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/Metrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/geometry/ChordGeometry.h>
#include <mxml/geometry/NoteGeometry.h>

#include <memory>


namespace mxml {

class ChordGeometry;

class ChordGeometryFactory {
public:
    ChordGeometryFactory(const ScoreProperties& scoreProperties, const Metrics& metrics);

    std::unique_ptr<ChordGeometry> build(const dom::Chord& chord);

private:
    Rect buildNotes(const dom::Chord& chord);
    void buildDot(const NoteGeometry& noteGeom);
    void buildAccidentals(const Rect& notesFrame);
    void buildAccidental(const NoteGeometry& noteGeom, const Rect& notesFrame);
    void buildNotations(const dom::Chord& chord, const Rect& notesFrame);
    void buildArticulation(const dom::Chord& chord, const dom::Articulation& articulation, Rect& notesFrame);
    void buildFermata(const dom::Fermata& fermata, Rect& notesFrame);
    void buildStem(const dom::Chord& chord);
    
private:
    const ScoreProperties& _scoreProperties;
    const Metrics& _metrics;

    std::unique_ptr<ChordGeometry> _geometry;
};

} // namespace mxml
