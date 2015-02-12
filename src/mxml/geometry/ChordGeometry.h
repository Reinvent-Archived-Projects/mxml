//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "MeasureElementGeometry.h"
#include "AccidentalGeometry.h"
#include "ArticulationGeometry.h"
#include "DotGeometry.h"
#include "NoteGeometry.h"
#include "StemGeometry.h"

#include <mxml/Metrics.h>
#include <mxml/ScoreProperties.h>
#include <mxml/dom/Chord.h>

namespace mxml {

class ChordGeometry : public MeasureElementGeometry {
public:
    static const coord_t kStemWidth;
    static const coord_t kStemVerticalOffset;
    static const coord_t kAccidentalSpacing;
    static const coord_t kArticulationSpacing;
    static const coord_t kFermataSpacing;
    static const coord_t kDotSpacing;
    
public:
    ChordGeometry(const dom::Chord& chord, const ScoreProperties& scoreProperties, const Metrics& metrics);
    
    const dom::Chord& chord() const {
        return _chord;
    }
    
    bool showFlags() const {
        if (!_stem)
            return false;
        return _stem->showFlags();
    }
    void setShowFlags(bool show) {
        if (_stem) {
            _stem->setShowFlags(show);
            setBounds(subGeometriesFrame());
        }
    }
    
    const std::vector<NoteGeometry*> notes() const {
        return _notes;
    }
    const StemGeometry* stem() const {
        return _stem;
    }
    
    /** Get the notes' frame without stems or annotations. */
    Rect notesFrame() const;

    /**
     Get the reference note location. The reference note is not displaced sideways and is usually the note at the
     edge of the stem. Used mainly for placing the stem.
     */
    Point refNoteLocation() const {
        return _refNoteLocation;
    }

    void extendStem(coord_t coordinate);
    
private:
    void build();
    Rect buildNotes();
    void buildDot(const NoteGeometry& noteGeom);
    void buildAccidentals(const Rect& notesFrame);
    void buildAccidental(const NoteGeometry& noteGeom, const Rect& notesFrame);
    void buildNotations(const Rect& notesFrame);
    void buildArticulation(const dom::Articulation& articulation, Rect& notesFrame);
    void buildFermata(const dom::Fermata& fermata, Rect& notesFrame);
    void buildStem();
    
private:
    const dom::Chord& _chord;
    const ScoreProperties& _scoreProperties;
    const Metrics& _metrics;
    
    std::vector<NoteGeometry*> _notes;
    StemGeometry* _stem;
    Point _refNoteLocation;
    int _currentAlter;
};

} // namespace mxml
