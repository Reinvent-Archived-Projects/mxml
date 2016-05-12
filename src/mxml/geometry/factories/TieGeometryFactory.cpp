// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "TieGeometryFactory.h"

#include "ChordGeometry.h"
#include "ClefGeometry.h"
#include "KeyGeometry.h"
#include "MeasureGeometry.h"
#include "NoteGeometry.h"

#include <mxml/Metrics.h>

namespace mxml {

TieGeometryFactory::TieGeometryFactory(const Geometry& parentGeometry, const Metrics& metrics)
: _parentGeometry(parentGeometry),
  _metrics(metrics)
{
}

std::vector<std::unique_ptr<TieGeometry>>&& TieGeometryFactory::buildTieGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries) {
    _tieGeometries.clear();
    _tieStartGeometries.clear();
    _slurStartGeometries.clear();
    createGeometries(geometries);

    // Finish any ties that started but did not stop
    for (auto& pair : _tieStartGeometries) {
        auto tie = pair.second.first;
        auto startGeom = pair.second.second;
        auto tieGeom = buildTieGeometryToEdge(startGeom, tie->placement());
        startGeom->setTieGeometry(tieGeom.get());
        _tieGeometries.push_back(std::move(tieGeom));
    }

    // Finish any slurs that started but did not stop
    for (auto& pair : _slurStartGeometries) {
        auto slur = pair.second.first;
        auto startGeom = pair.second.second;
        auto slurGeom = buildSlurGeometryToEdge(startGeom, slur->placement());
        startGeom->setTieGeometry(slurGeom.get());
        _tieGeometries.push_back(std::move(slurGeom));
    }

    return std::move(_tieGeometries);
}

void TieGeometryFactory::createGeometries(const std::vector<std::unique_ptr<Geometry>>& geometries) {
    for (auto& geom : geometries) {
        if (auto measure = dynamic_cast<MeasureGeometry*>(geom.get())) {
            createGeometries(measure->geometries());
        } else if (auto chordGeom = dynamic_cast<ChordGeometry*>(geom.get())) {
            createGeometriesFromChord(chordGeom);
        } else if (auto noteGeom = dynamic_cast<NoteGeometry*>(geom.get())) {
            createGeometryFromNote(noteGeom);
        }
    }
}

void TieGeometryFactory::createGeometriesFromChord(ChordGeometry* chord) {
    for (auto& note : chord->notes()) {
        createGeometryFromNote(note);
    }
}

void TieGeometryFactory::createGeometryFromNote(NoteGeometry* noteGeometry) {
    const dom::Note& note = noteGeometry->note();
    if (!note.notations)
        return;
    
    const auto& notations = note.notations;

    // Build ties
    for (auto& tie : notations->ties) {
        auto key = std::make_pair(note.staff(), note.pitch.get());
        if (tie->type() == dom::kStart || tie->type() == dom::kContinue) {
            _tieStartGeometries[key] = {tie.get(), noteGeometry};
        } else if (tie->type() == dom::kContinue) {
            buildTieGeometry(key, *noteGeometry, *tie);
            _tieStartGeometries[key] = {tie.get(), noteGeometry};
        } else if (tie->type() == dom::kStop) {
            buildTieGeometry(key, *noteGeometry, *tie);
        }
    }

    // Build slurs
    for (auto& slur : notations->slurs) {
        auto key = std::make_pair(note.staff(), slur->number());
        if (slur->type() == dom::kStart) {
            _slurStartGeometries[key] = {slur.get(), noteGeometry};
        } else if (slur->type() == dom::kContinue) {
            buildSlurGeometry(key, *noteGeometry, *slur);
            _slurStartGeometries[key] = {slur.get(), noteGeometry};
        } else if (slur->type() == dom::kStop) {
            buildSlurGeometry(key, *noteGeometry, *slur);
        }
    }
}

void TieGeometryFactory::buildTieGeometry(const PitchKey& key, NoteGeometry& noteGeometry, const dom::Tied& tie) {
    auto it = _tieStartGeometries.find(key);
    auto startGeom = it->second.second;

    std::unique_ptr<TieGeometry> tieGeom;
    if (it == _tieStartGeometries.end()) {
        tieGeom = buildTieGeometryFromEdge(&noteGeometry, tie.placement());
    } else {
        tieGeom = buildTieGeometry(startGeom, &noteGeometry, tie.placement());
        startGeom->setTieGeometry(tieGeom.get());
        _tieStartGeometries.erase(it);
    }
    noteGeometry.setTieGeometry(tieGeom.get());
    _tieGeometries.push_back(std::move(tieGeom));
}

void TieGeometryFactory::buildSlurGeometry(const SlurKey& key, NoteGeometry& noteGeometry, const dom::Slur& slur) {
    auto it = _slurStartGeometries.find(key);
    auto startGeom = it->second.second;

    std::unique_ptr<TieGeometry> slurGeom;
    if (it == _slurStartGeometries.end()) {
        slurGeom = buildSlurGeometryFromEdge(&noteGeometry, slur.placement());
    } else {
        slurGeom = buildSlurGeometry(startGeom, &noteGeometry, slur.placement());
        startGeom->setTieGeometry(slurGeom.get());
        _slurStartGeometries.erase(it);
    }
    noteGeometry.setTieGeometry(slurGeom.get());
    _tieGeometries.push_back(std::move(slurGeom));
}

std::unique_ptr<TieGeometry> TieGeometryFactory::buildTieGeometry(const NoteGeometry* start, const NoteGeometry* stop, const dom::Optional<dom::Placement>& placement) {
    std::unique_ptr<TieGeometry> tieGeom(new TieGeometry);
    
    Point startLocation;
    startLocation.x = start->frame().max().x;
    startLocation.y = start->center().y;

    Point stopLocation;
    stopLocation.x = stop->frame().min().x;
    stopLocation.y = stop->center().y;

    if (!placement.isPresent()) {
        coord_t startStaffY = startLocation.y - _metrics.staffOrigin(start->note().staff());
        coord_t stopStaffY = stopLocation.y - _metrics.staffOrigin(stop->note().staff());
        coord_t avgy = (startStaffY + stopStaffY) / 2;
        if (avgy < Metrics::staffHeight()/2)
            tieGeom->setPlacement(absentOptional(dom::Placement::Above));
        else
            tieGeom->setPlacement(absentOptional(dom::Placement::Below));
    }
    
    if (tieGeom->placement().value() == dom::Placement::Below) {
        startLocation.y = start->frame().max().y;
        stopLocation.y = stop->frame().max().y;
    } else {
        startLocation.y = start->frame().min().y;
        stopLocation.y = stop->frame().min().y;
    }

    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, start->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, stop->parentGeometry()));
    
    return tieGeom;
}

std::unique_ptr<TieGeometry> TieGeometryFactory::buildTieGeometryFromEdge(const NoteGeometry* stop, const dom::Optional<dom::Placement>& placement) {
    std::unique_ptr<TieGeometry> tieGeom(new TieGeometry);

    const Geometry* startGeometry = nullptr;
    std::vector<std::type_index> types = {std::type_index(typeid(KeyGeometry)), std::type_index(typeid(ClefGeometry))};

    coord_t stopX = stop->convertToGeometry(stop->frame().min(), &_parentGeometry).x;
    _parentGeometry.lookUpGeometriesWithTypes(types, [&startGeometry, stopX](const Geometry* geometry){
        if (!startGeometry || (geometry->frame().max().x < stopX && geometry->frame().max().x > startGeometry->frame().max().x))
            startGeometry = geometry;
    });

    Point startLocation;
    startLocation.x = startGeometry->convertToGeometry(startGeometry->bounds().max(), stop->parentGeometry()).x;
    startLocation.y = stop->center().y;

    Point stopLocation;
    stopLocation.x = stop->frame().min().x;
    stopLocation.y = stop->center().y;

    if (!placement.isPresent()) {
        coord_t staffY = startLocation.y - _metrics.staffOrigin(stop->note().staff());
        if (staffY < Metrics::staffHeight()/2)
            tieGeom->setPlacement(absentOptional(dom::Placement::Above));
        else
            tieGeom->setPlacement(absentOptional(dom::Placement::Below));
    }

    if (tieGeom->placement().value() == dom::Placement::Below) {
        stopLocation.y = stop->frame().max().y;
    } else {
        stopLocation.y = stop->frame().min().y;
    }
    startLocation.y = stopLocation.y;

    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, stop->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, stop->parentGeometry()));
    
    return tieGeom;
}

std::unique_ptr<TieGeometry> TieGeometryFactory::buildTieGeometryToEdge(const NoteGeometry* start, const dom::Optional<dom::Placement>& placement) {
    std::unique_ptr<TieGeometry> tieGeom(new TieGeometry);

    Point startLocation;
    startLocation.x = start->frame().max().x;
    startLocation.y = start->center().y;

    Point stopLocation;
    stopLocation.x = start->convertFromGeometry(_parentGeometry.bounds().max(), &_parentGeometry).x;
    stopLocation.x += start->frame().origin.x - kTieSpacing;
    stopLocation.y = start->center().y;

    if (!placement.isPresent()) {
        coord_t staffY = startLocation.y - _metrics.staffOrigin(start->note().staff());
        if (staffY < Metrics::staffHeight()/2)
            tieGeom->setPlacement(absentOptional(dom::Placement::Above));
        else
            tieGeom->setPlacement(absentOptional(dom::Placement::Below));
    }

    if (tieGeom->placement().value() == dom::Placement::Below) {
        startLocation.y = start->frame().max().y;
    } else {
        startLocation.y = start->frame().min().y;
    }
    stopLocation.y = startLocation.y;

    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, start->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, start->parentGeometry()));
    
    return tieGeom;
}


std::unique_ptr<TieGeometry> TieGeometryFactory::buildSlurGeometryFromEdge(const NoteGeometry* stop, const dom::Optional<dom::Placement>& placement) {
    std::unique_ptr<TieGeometry> tieGeom(new TieGeometry);

    const Geometry* startGeometry = nullptr;
    std::vector<std::type_index> types = {std::type_index(typeid(KeyGeometry)), std::type_index(typeid(ClefGeometry))};

    coord_t stopX = stop->convertToGeometry(stop->frame().min(), &_parentGeometry).x;
    _parentGeometry.lookUpGeometriesWithTypes(types, [&startGeometry, stopX](const Geometry* geometry){
        if (!startGeometry || (geometry->frame().max().x < stopX && geometry->frame().max().x > startGeometry->frame().max().x))
            startGeometry = geometry;
    });

    Point startLocation;
    startLocation.x = startGeometry->convertToGeometry(startGeometry->bounds().max(), stop->parentGeometry()).x + kTieSpacing;
    startLocation.y = stop->center().y;

    Point stopLocation = stop->location();
    stopLocation.x -= kTieSpacing;

    const ChordGeometry& stopChordGeom = static_cast<const ChordGeometry&>(*stop->parentGeometry());

    if (!placement.isPresent()) {
        auto stem = stopChordGeom.stem();
        if (stem && stem->stemDirection() == dom::Stem::Up)
            tieGeom->setPlacement(absentOptional(dom::Placement::Below));
        else
            tieGeom->setPlacement(absentOptional(dom::Placement::Above));
    }

    Rect stopChordFrame = stopChordGeom.frame();

    // Correctly place slur using depending on the stem direction
    if (tieGeom->placement().value() == dom::Placement::Below) {
        if (stopChordGeom.stem() && stopChordGeom.stem()->stemDirection() == dom::Stem::Down) {
            stopLocation.y = stopChordFrame.max().y - kSlurStemOffset;
        } else {
            stopLocation.y = stopChordFrame.max().y + kTieSpacing;
        }
    } else {
        if (stopChordGeom.stem() && stopChordGeom.stem()->stemDirection() == dom::Stem::Up) {
            stopLocation.y = stopChordFrame.min().y + kSlurStemOffset;
        } else {
            stopLocation.y = stopChordFrame.min().y - kTieSpacing;
        }
    }
    startLocation.y = stopLocation.y;

    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, stop->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, stop->parentGeometry()));

    return tieGeom;
}

std::unique_ptr<TieGeometry> TieGeometryFactory::buildSlurGeometryToEdge(const NoteGeometry* start, const dom::Optional<dom::Placement>& placement) {
    std::unique_ptr<TieGeometry> tieGeom(new TieGeometry);

    Point startLocation = start->location();
    startLocation.x += kTieSpacing;

    Point stopLocation;
    stopLocation.x = start->convertFromGeometry(_parentGeometry.bounds().max(), &_parentGeometry).x;
    stopLocation.x += start->frame().origin.x - kTieSpacing;
    stopLocation.y = start->center().y;


    const ChordGeometry& startChordGeom = static_cast<const ChordGeometry&>(*start->parentGeometry());

    if (!placement.isPresent()) {
        auto stem = startChordGeom.stem();
        if (stem && stem->stemDirection() == dom::Stem::Up)
            tieGeom->setPlacement(absentOptional(dom::Placement::Below));
        else
            tieGeom->setPlacement(absentOptional(dom::Placement::Above));
    }

    Rect startChordFrame = startChordGeom.frame();

    // Correctly place slur using depending on the stem direction
    if (tieGeom->placement().value() == dom::Placement::Below) {
        if (startChordGeom.stem() && startChordGeom.stem()->stemDirection() == dom::Stem::Down) {
            startLocation.y = startChordFrame.max().y - kSlurStemOffset;
            startLocation.x = start->frame().max().x - kTieSpacing;
        } else {
            startLocation.y = startChordFrame.max().y + kTieSpacing;
        }
    } else {
        if (startChordGeom.stem() && startChordGeom.stem()->stemDirection() == dom::Stem::Up) {
            startLocation.y = startChordFrame.min().y + kSlurStemOffset;
            startLocation.x = start->frame().max().x + kTieSpacing;
        } else {
            startLocation.y = startChordFrame.min().y - kTieSpacing;
        }
    }
    stopLocation.y = startLocation.y;

    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, start->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, start->parentGeometry()));
    
    return tieGeom;
}

std::unique_ptr<TieGeometry> TieGeometryFactory::buildSlurGeometry(const NoteGeometry* start, const NoteGeometry* stop, const dom::Optional<dom::Placement>& placement) {
    std::unique_ptr<TieGeometry> tieGeom(new TieGeometry);
    
    Point startLocation = start->location();
    Point stopLocation = stop->location();
    
    startLocation.x += kTieSpacing;
    stopLocation.x -= kTieSpacing;

    const ChordGeometry& startChordGeom = static_cast<const ChordGeometry&>(*start->parentGeometry());
    const ChordGeometry& stopChordGeom = static_cast<const ChordGeometry&>(*stop->parentGeometry());

    if (!placement.isPresent()) {
        auto s1 = startChordGeom.stem();
        auto s2 = stopChordGeom.stem();

        if (s1 && s1->stemDirection() == dom::Stem::Up && s2 && s2->stemDirection() == dom::Stem::Up)
            tieGeom->setPlacement(absentOptional(dom::Placement::Below));
        else
            tieGeom->setPlacement(absentOptional(dom::Placement::Above));
    }
    
    Rect startChordFrame = startChordGeom.frame();
    Rect stopChordFrame = stopChordGeom.frame();

    // Correctly place slur using depending on the stem direction
    if (tieGeom->placement().value() == dom::Placement::Below) {
        if (startChordGeom.stem() && startChordGeom.stem()->stemDirection() == dom::Stem::Down) {
            startLocation.y = startChordFrame.max().y - kSlurStemOffset;
            startLocation.x = start->frame().max().x - kTieSpacing;
        } else {
            startLocation.y = startChordFrame.max().y + kTieSpacing;
        }
        if (stopChordGeom.stem() && stopChordGeom.stem()->stemDirection() == dom::Stem::Down) {
            stopLocation.y = stopChordFrame.max().y - kSlurStemOffset;
            startLocation.x = start->frame().max().x + kTieSpacing;
        } else {
            stopLocation.y = stopChordFrame.max().y + kTieSpacing;
        }
    } else {
        if (startChordGeom.stem() && startChordGeom.stem()->stemDirection() == dom::Stem::Up) {
            startLocation.y = startChordFrame.min().y + kSlurStemOffset;
            startLocation.x = start->frame().max().x + kTieSpacing;
        } else {
            startLocation.y = startChordFrame.min().y - kTieSpacing;
        }
        if (stopChordGeom.stem() && stopChordGeom.stem()->stemDirection() == dom::Stem::Up) {
            stopLocation.y = stopChordFrame.min().y + kSlurStemOffset;
            startLocation.x = start->frame().max().x + kTieSpacing;
        } else {
            stopLocation.y = stopChordFrame.min().y - kTieSpacing;
        }
    }

    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, start->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, stop->parentGeometry()));
    
    return tieGeom;
}

} // namespace mxml
