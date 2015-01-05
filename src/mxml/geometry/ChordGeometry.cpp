//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "ChordGeometry.h"
#include "PartGeometry.h"
#include "FermataGeometry.h"

#include <mxml/Metrics.h>

namespace mxml {

const coord_t ChordGeometry::kStemVerticalOffset = 1;
const coord_t ChordGeometry::kAccidentalSpacing = 2;
const coord_t ChordGeometry::kArticulationSpacing = 1;
const coord_t ChordGeometry::kFermataSpacing = 11;
const coord_t ChordGeometry::kDotSpacing = 2;

ChordGeometry::ChordGeometry(const dom::Chord& chord, const AttributesManager& attributesManager, const PartGeometry& partGeometry)
: _chord(chord), _attributesManager(attributesManager), _partGeometry(partGeometry), _notes(), _stem() {
    build();
}

Rect ChordGeometry::notesFrame() const {
    bool first = true;
    Rect frame;
    for (const NoteGeometry* noteGeom : _notes) {
        if (first)
            frame = noteGeom->frame();
        else
            frame = join(frame, noteGeom->frame());
        first = false;
    }
    return frame;
}

void ChordGeometry::extendStem(coord_t coordinate) {
    coord_t dy;
    Rect f = frame();
    if (_chord.stem() == dom::STEM_UP || _chord.stem() == dom::STEM_DOUBLE) {
        dy = f.min().y - coordinate;
    } else {
        dy = coordinate - f.max().y;
    }

    if (_stem) {
        Size size{_stem->size().width, _stem->size().height + dy};
        _stem->setSize(size);
        setBounds(subGeometriesFrame());
    }
}

void ChordGeometry::build() {
    Rect notesFrame = buildNotes();
    buildAccidentals(notesFrame);
    buildNotations(notesFrame);
    buildStem();

    setBounds(subGeometriesFrame());
}

Rect ChordGeometry::buildNotes() {
    for (auto& note : _chord.notes()) {
        std::unique_ptr<NoteGeometry> geom(new NoteGeometry(*note));
        Point loc = {0, Metrics::noteY(_attributesManager, *note)};
        geom->setLocation(loc);
        
        _notes.push_back(geom.get());
        addGeometry(std::move(geom));
    }

    // Sort notes by stem position
    dom::Stem stem = _chord.stem();
    std::vector<NoteGeometry*> sorted = _notes;
    std::sort(sorted.begin(), sorted.end(), [stem](const NoteGeometry* g1, const NoteGeometry* g2) {
        if (stem == dom::STEM_UP)
            return g1->location().y < g2->location().y;
        else
            return g1->location().y > g2->location().y;
    });

    Rect notesFrame;
    for (std::size_t i = 0; i < sorted.size(); i += 1) {
        NoteGeometry* geom = sorted[i];
        Point loc = geom->location();

        // Notes that are too close need to be shifted sideways
        for (std::size_t j = i + 1; j < sorted.size(); j += 1) {
            NoteGeometry* noteGeom = sorted[j];
            if (loc.x == noteGeom->location().x && std::abs(loc.y - noteGeom->location().y) < Metrics::kStaffLineSpacing) {
                if (stem == dom::STEM_UP)
                    loc.x += geom->size().width;
                else
                    loc.x -= geom->size().width;
            }
        }
        geom->setLocation(loc);
        _refNoteLocation = loc;

        if (notesFrame.size.width == 0) {
            notesFrame = geom->frame();
        } else {
            notesFrame = join(notesFrame, geom->frame());
        }
    }

    // Build dots
    for (auto& noteGeometry : _notes) {
        buildDot(*noteGeometry);
    }
    
    return notesFrame;
}

void ChordGeometry::buildDot(const NoteGeometry& noteGeom) {
    const dom::Note& note = noteGeom.note();
    if (!note.dot())
        return;

    std::unique_ptr<DotGeometry> dotGeom(new DotGeometry);
    
    Point dotLocation = noteGeom.location();
    dotLocation.x += noteGeom.size().width/2 + dotGeom->anchorPoint().x + kDotSpacing;
    
    coord_t staffY = dotLocation.y - Metrics::staffOrigin(_partGeometry.part(), note.staff());
    if ((int)staffY % 10 == 0) {
        if (note.dot()->placement() == dom::PLACEMENT_ABOVE)
            dotLocation.y -= 5;
        else
            dotLocation.y += 5;
    }
    dotGeom->setLocation(dotLocation);

    for (auto& noteGeometry : _notes) {
        if (intersect(noteGeometry->frame(), dotGeom->frame()))
            dotLocation.x = noteGeometry->frame().max().x + dotGeom->anchorPoint().x + kDotSpacing;
    }
    dotGeom->setLocation(dotLocation);
    
    addGeometry(std::move(dotGeom));
}

void ChordGeometry::buildAccidentals(const Rect& notesFrame) {
    for (auto& geom : _notes)
        buildAccidental(*geom, notesFrame);
}

void ChordGeometry::buildAccidental(const NoteGeometry& noteGeom, const Rect& notesFrame) {
    const dom::Note& note = noteGeom.note();
    if (!note.accidental())
        return;

    std::unique_ptr<AccidentalGeometry> accGeom(new AccidentalGeometry(*note.accidental()));
    
    Point accLocation = noteGeom.location();
    accLocation.x = notesFrame.origin.x - kAccidentalSpacing - accGeom->size().width/2;
    accGeom->setLocation(accLocation);
    
    addGeometry(std::move(accGeom));
}

void ChordGeometry::buildNotations(const Rect& notesFrame) {
    if (_chord.notes().empty())
        return;
    
    // Notations should be on the first note of the chord
    const dom::Note* note = _chord.notes().front().get();
    
    if (!note || !note->notations())
        return;

    Rect frame = notesFrame;
    const auto& notations = note->notations();
    for (auto& artic : notations->articulations()) {
        buildArticulation(*artic, frame);
    }

    if (notations->fermata()) {
        buildFermata(*notations->fermata(), frame);
    }
}

void ChordGeometry::buildArticulation(const dom::Articulation& articulation, Rect& notesFrame) {
    const int staff = _chord.firstNote()->staff();
    std::unique_ptr<ArticulationGeometry> geom(new ArticulationGeometry(articulation, _chord.stem()));
    
    bool above;
    if (articulation.placement().isPresent()) {
        above = articulation.placement() == dom::PLACEMENT_ABOVE;
    } else {
        above = _chord.stem() != dom::STEM_UP && _chord.stem() != dom::STEM_DOUBLE;
    }
    
    Size size = geom->size();
    coord_t staffY;
    Point location;
    location.x = notesFrame.center().x;
    if (above) {
        location.y = notesFrame.origin.y - size.height/2 - kArticulationSpacing;
        staffY = notesFrame.origin.y - Metrics::staffOrigin(_partGeometry.part(), staff);
    } else {
        location.y = notesFrame.max().y + size.height/2 + kArticulationSpacing;
        staffY = notesFrame.max().y - Metrics::staffOrigin(_partGeometry.part(), staff);
    }

    // Add extra spacing if there is a staff line between the note and the articulation
    if ((int)staffY % 10 == 0) {
        if (above)
            location.y -= kArticulationSpacing;
        else
            location.y += kArticulationSpacing;
    }

    // Avoid placing the on a staff line
    bool intersectsStaffLine = false;
    coord_t y = Metrics::staffOrigin(_partGeometry.part(), staff);
    for (int i = 1; i <= Metrics::kStaffLineCount; i += 1) {
        if (location.y - size.height/2 < y && location.y + size.height/2 > y) {
            intersectsStaffLine = true;
            break;
        }
        y += Metrics::kStaffLineSpacing;
    }
    if (intersectsStaffLine) {
        if (above)
            location.y = y - Metrics::kStaffLineSpacing/2;
        else
            location.y = y + Metrics::kStaffLineSpacing/2;
    }
    
    geom->setLocation(location);
    notesFrame = join(notesFrame, geom->frame());
    
    addGeometry(std::move(geom));
}

void ChordGeometry::buildFermata(const dom::Fermata& fermata, Rect& notesFrame) {
    std::unique_ptr<FermataGeometry> geom(new FermataGeometry(fermata));

    Size size = geom->size();
    Point location;
    location.x = _refNoteLocation.x;

    bool above = fermata.type() == dom::Fermata::TYPE_UPRIGHT;
    if (above) {
        location.y = notesFrame.origin.y - size.height/2 - kFermataSpacing;
    } else {
        location.y = notesFrame.max().y + size.height/2 + kFermataSpacing;
    }
    
    geom->setLocation(location);
    notesFrame = join(notesFrame, geom->frame());
    
    addGeometry(std::move(geom));
}

void ChordGeometry::buildStem() {
    const dom::Note* note = _chord.firstNote();
    if (note->type() > dom::Note::TYPE_HALF || (note->stem() != dom::STEM_UP && note->stem() != dom::STEM_DOWN))
        return;
    
    bool flags = note->beams().empty() && _chord.firstNote()->beams().empty();
    unique_ptr<StemGeometry> stem(new StemGeometry(*note, flags));
    _stem = stem.get();
    addGeometry(std::move(stem));
    
    // Find edge notes
    bool first = true;
    Point topLocation;
    Point bottomLocation;
    for (auto note : _notes) {
        if (first || note->location().y < topLocation.y)
            topLocation = note->location();
        if (first || note->location().y > bottomLocation.y)
            bottomLocation = note->location();
        first = false;
    }
    
    // Set the location of the stem
    Point stemLocation;
    if (_chord.stem() == dom::STEM_UP) {
        stemLocation = {_refNoteLocation.x, topLocation.y};
    } else {
        stemLocation = {_refNoteLocation.x, bottomLocation.y};
    }
    _stem->setLocation(stemLocation);

    _stem->setHorizontalAnchorPointValues(0, NoteGeometry::kQuarterWidth/2);
    if (_chord.stem() == dom::STEM_UP) {
        _stem->setVerticalAnchorPointValues(1, -NoteGeometry::kHeight/2);
    } else {
        _stem->setVerticalAnchorPointValues(0, NoteGeometry::kHeight/2);
    }
}

} // namespace mxml
