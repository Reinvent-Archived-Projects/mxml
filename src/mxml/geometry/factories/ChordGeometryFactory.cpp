// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ChordGeometryFactory.h"
#include <mxml/geometry/FermataGeometry.h>


namespace mxml {

ChordGeometryFactory::ChordGeometryFactory(const ScoreProperties& scoreProperties, const Metrics& metrics)
: _scoreProperties(scoreProperties),
  _metrics(metrics)
{
}

std::unique_ptr<ChordGeometry> ChordGeometryFactory::build(const dom::Chord& chord) {
    if (!chord.firstNote() || !chord.firstNote()->printObject)
        return {};

    _geometry.reset(new ChordGeometry{chord});

    Rect notesFrame = buildNotes(chord);
    buildAccidentals(notesFrame);
    buildNotations(chord, notesFrame);
    buildStem(chord);

    _geometry->setBounds(_geometry->subGeometriesFrame());

    return std::move(_geometry);
}

void ChordGeometryFactory::resetForStem(ChordGeometry* chordGeometry) {
    if (!chordGeometry->stem())
        return;

    Rect notesFrame = placeNotes(chordGeometry);
    for (auto& geom : chordGeometry->geometries()) {
        if (auto articulationGeometry = dynamic_cast<ArticulationGeometry*>(geom.get()))
            placeArticulation(chordGeometry, articulationGeometry, notesFrame);
    }
    placeStem(chordGeometry);

    chordGeometry->setBounds(chordGeometry->subGeometriesFrame());
}

Rect ChordGeometryFactory::buildNotes(const dom::Chord& chord) {
    for (auto& note : chord.notes()) {
        std::unique_ptr<NoteGeometry> geom(new NoteGeometry(*note));
        Point loc{0, _metrics.noteY(*note)};
        geom->setLocation(loc);

        _geometry->_notes.push_back(geom.get());
        _geometry->addGeometry(std::move(geom));
    }

    // Build dots
    for (auto& noteGeometry : _geometry->_notes) {
        buildDot(*noteGeometry);
    }

    return placeNotes(_geometry.get());
}

void ChordGeometryFactory::buildDot(const NoteGeometry& noteGeom) {
    const dom::Note& note = noteGeom.note();
    if (!note.dot)
        return;

    std::unique_ptr<DotGeometry> dotGeom(new DotGeometry);

    Point dotLocation = noteGeom.location();
    dotLocation.x += noteGeom.size().width/2 + dotGeom->anchorPoint().x + ChordGeometry::kDotSpacing;

    coord_t staffY = dotLocation.y - _metrics.staffOrigin(note.staff());
    if ((int)staffY % 10 == 0) {
        if (note.dot->placement() == dom::Placement::Above)
            dotLocation.y -= 5;
        else
            dotLocation.y += 5;
    }
    dotGeom->setLocation(dotLocation);

    for (auto& noteGeometry : _geometry->_notes) {
        if (intersect(noteGeometry->frame(), dotGeom->frame()))
            dotLocation.x = noteGeometry->frame().max().x + dotGeom->anchorPoint().x + ChordGeometry::kDotSpacing;
    }
    dotGeom->setLocation(dotLocation);

    _geometry->addGeometry(std::move(dotGeom));
}

void ChordGeometryFactory::buildAccidentals(const Rect& notesFrame) {
    for (auto& geom : _geometry->_notes)
        buildAccidental(*geom, notesFrame);
}

void ChordGeometryFactory::buildAccidental(const NoteGeometry& noteGeom, const Rect& notesFrame) {
    const dom::Note& note = noteGeom.note();
    if (!note.pitch)
        return;

    int alter = note.alter();
    int previousAlter = _scoreProperties.alter(note);
    if (alter == previousAlter)
        return;

    std::unique_ptr<AccidentalGeometry> accGeom(new AccidentalGeometry(alter));

    Point accLocation = noteGeom.location();
    accLocation.x = notesFrame.origin.x - ChordGeometry::kAccidentalSpacing - accGeom->size().width/2;
    accGeom->setLocation(accLocation);

    _geometry->addGeometry(std::move(accGeom));
}

void ChordGeometryFactory::buildNotations(const dom::Chord& chord, const Rect& notesFrame) {
    if (chord.notes().empty())
        return;

    // Notations should be on the first note of the chord
    const dom::Note* note = chord.notes().front().get();

    if (!note || !note->notations)
        return;

    Rect frame = notesFrame;
    const auto& notations = note->notations;
    for (auto& artic : notations->articulations) {
        buildArticulation(chord, *artic, frame);
    }

    if (notations->fermata) {
        buildFermata(*notations->fermata, frame);
    }
}

void ChordGeometryFactory::buildArticulation(const dom::Chord& chord, const dom::Articulation& articulation, Rect& notesFrame) {
    std::unique_ptr<ArticulationGeometry> geom(new ArticulationGeometry(articulation, chord.stem()));
    placeArticulation(_geometry.get(), geom.get(), notesFrame);
    _geometry->addGeometry(std::move(geom));
}

void ChordGeometryFactory::buildFermata(const dom::Fermata& fermata, Rect& notesFrame) {
    std::unique_ptr<FermataGeometry> geom(new FermataGeometry(fermata));

    Size size = geom->size();
    Point location;
    location.x = _geometry->_refNoteLocation.x;

    bool above = fermata.type() == dom::Fermata::Type::Upright;
    if (above) {
        location.y = notesFrame.origin.y - size.height/2 - ChordGeometry::kFermataSpacing;
    } else {
        location.y = notesFrame.max().y + size.height/2 + ChordGeometry::kFermataSpacing;
    }

    geom->setLocation(location);
    notesFrame = join(notesFrame, geom->frame());

    _geometry->addGeometry(std::move(geom));
}

void ChordGeometryFactory::buildStem(const dom::Chord& chord) {
    const dom::Note* note = chord.firstNote();
    if (note->type() > dom::Note::Type::Half || (note->stem() != dom::Stem::Up && note->stem() != dom::Stem::Down))
        return;

    bool flags = note->beams().empty() && chord.firstNote()->beams().empty();
    std::unique_ptr<StemGeometry> stem(new StemGeometry(*note, flags));
    _geometry->_stem = stem.get();
    _geometry->addGeometry(std::move(stem));
    placeStem(_geometry.get());
}


#pragma mark - Placement

Rect ChordGeometryFactory::placeNotes(ChordGeometry* chordGeometry) {
    auto& chord = chordGeometry->chord();
    auto stem = chord.stem().value();
    if (chordGeometry->stem())
        stem = chordGeometry->stem()->stemDirection();

    // Sort notes by stem position
    std::vector<NoteGeometry*> sorted = chordGeometry->_notes;
    std::sort(sorted.begin(), sorted.end(), [stem](const NoteGeometry* g1, const NoteGeometry* g2) {
        if (stem == dom::Stem::Up)
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
                if (stem == dom::Stem::Up)
                    loc.x += geom->size().width;
                else
                    loc.x -= geom->size().width;
            }
        }
        geom->setLocation(loc);
        chordGeometry->_refNoteLocation = loc;

        if (notesFrame.size.width == 0) {
            notesFrame = geom->frame();
        } else {
            notesFrame = join(notesFrame, geom->frame());
        }
    }
    
    return notesFrame;
}

void ChordGeometryFactory::placeArticulation(ChordGeometry* chordGeometry, ArticulationGeometry* articulationGeometry, Rect& notesFrame) {
    auto staff = chordGeometry->chord().firstNote()->staff();
    auto& articulation = articulationGeometry->articulation();

    bool above = true;
    if (articulation.placement().isPresent()) {
        above = articulation.placement() == dom::Placement::Above;
    } else if (chordGeometry->stem()) {
        above = chordGeometry->stem()->stemDirection() == dom::Stem::Down;
    } else {
        above = chordGeometry->chord().stem() == dom::Stem::Down;
    }

    Size size = articulationGeometry->size();
    coord_t staffY;
    Point location;
    location.x = notesFrame.center().x;
    if (above) {
        location.y = notesFrame.origin.y - size.height/2 - ChordGeometry::kArticulationSpacing;
        staffY = notesFrame.origin.y - _metrics.staffOrigin(staff);
    } else {
        location.y = notesFrame.max().y + size.height/2 + ChordGeometry::kArticulationSpacing;
        staffY = notesFrame.max().y - _metrics.staffOrigin(staff);
    }

    // Add extra spacing if there is a staff line between the note and the articulation
    if ((int)staffY % 10 == 0) {
        if (above)
            location.y -= ChordGeometry::kArticulationSpacing;
        else
            location.y += ChordGeometry::kArticulationSpacing;
    }

    // Avoid placing the on a staff line
    bool intersectsStaffLine = false;
    coord_t y = _metrics.staffOrigin(staff);
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
    
    articulationGeometry->setLocation(location);
    notesFrame = join(notesFrame, articulationGeometry->frame());
}

void ChordGeometryFactory::placeStem(ChordGeometry* chordGeometry) {
    auto stem = chordGeometry->stem();
    if (!stem)
        return;

    // Find edge notes
    bool first = true;
    Point topLocation;
    Point bottomLocation;
    for (auto& note : chordGeometry->notes()) {
        if (first || note->location().y < topLocation.y)
            topLocation = note->location();
        if (first || note->location().y > bottomLocation.y)
            bottomLocation = note->location();
        first = false;
    }

    // Extend the stems of notes above and below ledger lines to staves center
    auto staffOrigin = _metrics.staffOrigin(chordGeometry->staff());
    auto stavesCenter = staffOrigin + _metrics.staffHeight()/2;
    if (stem->stemDirection() == dom::Stem::Up && stem->frame().min().y > stavesCenter)
        chordGeometry->extendStem(stavesCenter);
    if (stem->stemDirection() == dom::Stem::Down && stem->frame().max().y < stavesCenter)
        chordGeometry->extendStem(stavesCenter);

    // Set the location of the stem
    Point stemLocation;
    if (stem->stemDirection() == dom::Stem::Up) {
        stemLocation = {chordGeometry->refNoteLocation().x, topLocation.y};
        stem->setVerticalAnchorPointValues(1, -NoteGeometry::kHeight/2);
    } else {
        stemLocation = {chordGeometry->refNoteLocation().x, bottomLocation.y};
        stem->setVerticalAnchorPointValues(0, NoteGeometry::kHeight/2);
    }
    stem->setHorizontalAnchorPointValues(0, NoteGeometry::kQuarterWidth/2);
    stem->setLocation(stemLocation);
}

} // namespace mxml
