//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "TieGeometryFactory.h"
#include "ChordGeometry.h"
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
    if (!note.notations())
        return;
    
    const auto& notations = note.notations();
    
    for (auto& tie : notations->ties()) {
        auto key = std::make_pair(note.staff(), note.pitch().get());
        if (tie->type() == dom::kContinue) {
            _tieStartGeometries[key] = std::make_pair(tie.get(), noteGeometry);
        } else if (tie->type() == dom::kStop) {
            auto it = _tieStartGeometries.find(key);
            auto startGeom = it->second.second;

            std::unique_ptr<TieGeometry> tieGeom;
            if (it == _tieStartGeometries.end()) {
                tieGeom = buildTieGeometryFromEdge(noteGeometry, tie->placement());
            } else {
                tieGeom = buildTieGeometry(startGeom, noteGeometry, tie->placement());
                startGeom->setTieGeometry(tieGeom.get());
                _tieStartGeometries.erase(it);
            }
            noteGeometry->setTieGeometry(tieGeom.get());

            _tieGeometries.push_back(std::move(tieGeom));
        }
    }

    for (auto& slur : notations->slurs()) {
        auto key = std::make_pair(note.staff(), slur->number());
        if (slur->type() == dom::kStart || slur->type() == dom::kContinue) {
            _slurStartGeometries[key] = noteGeometry;
        } else if (slur->type() == dom::kStop) {
            auto startGeom = _slurStartGeometries.find(key);
            if (startGeom != _slurStartGeometries.end()) {
                std::unique_ptr<TieGeometry> slurGeom = buildSlurGeometry(startGeom->second, noteGeometry, slur->placement());
                _tieGeometries.push_back(std::move(slurGeom));
                startGeom->second->setTieGeometry(slurGeom.get());
                noteGeometry->setTieGeometry(slurGeom.get());
                
                _slurStartGeometries.erase(startGeom);
            }
        }
    }
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
            tieGeom->setPlacement(absentOptional(dom::kPlacementAbove));
        else
            tieGeom->setPlacement(absentOptional(dom::kPlacementBelow));
    }
    
    if (tieGeom->placement().value() == dom::kPlacementBelow) {
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

    Point startLocation;
    startLocation.x = stop->convertFromGeometry({0, 0}, &_parentGeometry).x;
    startLocation.y = stop->center().y;

    Point stopLocation;
    stopLocation.x = stop->frame().min().x;
    stopLocation.y = stop->center().y;

    if (!placement.isPresent()) {
        coord_t staffY = startLocation.y - _metrics.staffOrigin(stop->note().staff());
        if (staffY < Metrics::staffHeight()/2)
            tieGeom->setPlacement(absentOptional(dom::kPlacementAbove));
        else
            tieGeom->setPlacement(absentOptional(dom::kPlacementBelow));
    }

    if (tieGeom->placement().value() == dom::kPlacementBelow) {
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
    stopLocation.y = start->center().y;

    if (!placement.isPresent()) {
        coord_t staffY = startLocation.y - _metrics.staffOrigin(start->note().staff());
        if (staffY < Metrics::staffHeight()/2)
            tieGeom->setPlacement(absentOptional(dom::kPlacementAbove));
        else
            tieGeom->setPlacement(absentOptional(dom::kPlacementBelow));
    }

    if (tieGeom->placement().value() == dom::kPlacementBelow) {
        startLocation.y = start->frame().max().y;
    } else {
        startLocation.y = start->frame().min().y;
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

        if (s1 && s1->stemDirection() == dom::kStemUp && s2 && s2->stemDirection() == dom::kStemUp)
            tieGeom->setPlacement(absentOptional(dom::kPlacementBelow));
        else
            tieGeom->setPlacement(absentOptional(dom::kPlacementAbove));
    }
    
    Rect startChordFrame = startChordGeom.frame();
    Rect stopChordFrame = stopChordGeom.frame();
    
    if (tieGeom->placement().value() == dom::kPlacementBelow) {
        if (startChordGeom.stem() && startChordGeom.stem()->stemDirection() == dom::kStemDown) {
            startLocation.y = startChordFrame.max().y - kSlurStemOffset;
            startLocation.x = start->frame().max().x - kTieSpacing;
        } else {
            startLocation.y = startChordFrame.max().y + kTieSpacing;
        }
        if (stopChordGeom.stem() && stopChordGeom.stem()->stemDirection() == dom::kStemDown) {
            stopLocation.y = stopChordFrame.max().y - kSlurStemOffset;
            startLocation.x = start->frame().max().x + kTieSpacing;
        } else {
            stopLocation.y = stopChordFrame.max().y + kTieSpacing;
        }
    } else {
        if (startChordGeom.stem() && startChordGeom.stem()->stemDirection() == dom::kStemUp) {
            startLocation.y = startChordFrame.min().y + kSlurStemOffset;
            startLocation.x = start->frame().max().x + kTieSpacing;
        } else {
            startLocation.y = startChordFrame.min().y - kTieSpacing;
        }
        if (stopChordGeom.stem() && stopChordGeom.stem()->stemDirection() == dom::kStemUp) {
            stopLocation.y = stopChordFrame.min().y + kSlurStemOffset;
            startLocation.x = start->frame().max().x + kTieSpacing;
        } else {
            stopLocation.y = stopChordFrame.min().y - kTieSpacing;
        }
    }
    
    // Avoid collisions with beamed sets
    if (!start->note().beams().empty() && start->note().beams().front()->type() != dom::Beam::kTypeEnd) {
        ChordGeometry* chordGeom = (ChordGeometry*)start->parentGeometry();
        Rect stemFrame = chordGeom->stem()->frame();
        
        if (tieGeom->placement().value() == dom::kPlacementBelow && start->note().stem() == dom::kStemDown) {
            startLocation.x = stemFrame.min().x + StemGeometry::kNoFlagWidth;
            startLocation.y = stemFrame.max().y + 2*kTieSpacing;
        } else if (tieGeom->placement().value() == dom::kPlacementAbove && start->note().stem() == dom::kStemUp) {
            startLocation.x = stemFrame.max().x - StemGeometry::kNoFlagWidth;
            startLocation.y = stemFrame.min().y - 2*kTieSpacing;
        }
    }
    if (!stop->note().beams().empty() && stop->note().beams().front()->type() != dom::Beam::kTypeBegin) {
        ChordGeometry* chordGeom = (ChordGeometry*)stop->parentGeometry();
        Rect stemFrame = chordGeom->stem()->frame();
        
        if (tieGeom->placement().value() == dom::kPlacementBelow && stop->note().stem() == dom::kStemDown) {
            stopLocation.x = stemFrame.min().x + StemGeometry::kNoFlagWidth;
            stopLocation.y = stemFrame.max().y + 2*kTieSpacing;
        } else if (tieGeom->placement().value() == dom::kPlacementAbove && stop->note().stem() == dom::kStemUp) {
            stopLocation.x = stemFrame.max().x - StemGeometry::kNoFlagWidth;
            stopLocation.y = stemFrame.min().y - 2*kTieSpacing;
        }
    }

    tieGeom->setStartLocation(_parentGeometry.convertFromGeometry(startLocation, start->parentGeometry()));
    tieGeom->setStopLocation(_parentGeometry.convertFromGeometry(stopLocation, stop->parentGeometry()));
    
    return tieGeom;
}

} // namespace mxml
