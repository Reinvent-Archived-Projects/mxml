// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

    // Adjust placement of subgeometries based on new stem orientation
    void resetForStem(ChordGeometry* chordGeometry);

private:
    Rect buildNotes(const dom::Chord& chord);
    void buildDot(const NoteGeometry& noteGeom);
    void buildAccidentals(const Rect& notesFrame);
    void buildAccidental(const NoteGeometry& noteGeom, const Rect& notesFrame);
    void buildNotations(const dom::Chord& chord, const Rect& notesFrame);
    void buildArticulation(const dom::Chord& chord, const dom::Articulation& articulation, Rect& notesFrame);
    void buildFermata(const dom::Fermata& fermata, Rect& notesFrame);
    void buildStem(const dom::Chord& chord);

    Rect placeNotes(ChordGeometry* chordGeometry);
    void placeArticulation(ChordGeometry* chordGeometry, ArticulationGeometry* articulationGeometry, Rect& notesFrame);
    void placeStem(ChordGeometry* chordGeometry);

private:
    const ScoreProperties& _scoreProperties;
    const Metrics& _metrics;

    std::unique_ptr<ChordGeometry> _geometry;
};

} // namespace mxml
