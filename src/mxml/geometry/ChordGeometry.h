// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    ChordGeometry(const dom::Chord& chord);
    
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
    StemGeometry* stem() {
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
    const dom::Chord& _chord;
    
    std::vector<NoteGeometry*> _notes;
    StemGeometry* _stem;
    Point _refNoteLocation;

    friend class ChordGeometryFactory;
};

} // namespace mxml
