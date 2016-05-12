// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "StemDirectionResolver.h"

#include <mxml/EqualityConstraintSolver.h>
#include <cassert>
#include <string>
#include <unordered_map>
#include <vector>


namespace mxml {

StemDirectionResolver::StemDirectionResolver() {

}

void StemDirectionResolver::resolve(const MeasureGeometry* measureGeometry) {
    _measureGeometry = measureGeometry;

    buildChords();
    buildVariables();

    // First resolve independent chords and remove them from further consideration
    std::unordered_set<const ChordGeometry*> toRemove;
    for (auto& pair : _variables) {
        auto& var = pair.second;
        if (var.equal.empty() && var.opposite.empty()) {
            resolve(var.chordGeometry);
            toRemove.insert(pair.first);
        }
    }

    std::vector<ChordGeometry*> chords;
    std::unordered_map<const ChordGeometry*, std::size_t> indices;
    for (auto& pair : _variables) {
        auto chordGeometry = pair.first;
        if (toRemove.count(chordGeometry) == 0) {
            indices.insert({chordGeometry, chords.size()});
            chords.push_back(chordGeometry);
        }
    }

    // We are done if there is nothing left
    if (chords.empty())
        return;

    // Set up solver
    EqualityConstraintSolver solver;
    solver.setVariableCount(chords.size());
    for (auto& pair : _variables) {
        auto chordGeometry = pair.first;
        auto& var = pair.second;
        if (toRemove.count(chordGeometry) > 0)
            continue;

        for (auto& c : var.equal)
            solver.addEqualConstraint(indices[chordGeometry], indices[c]);
        for (auto& c : var.opposite)
            solver.addDifferentConstraint(indices[chordGeometry], indices[c]);
    }

    // Solve
    int minValue = std::numeric_limits<int>::max();
    std::vector<bool> solution;
    auto count = solver.solve([&](const std::vector<bool>& values) {
        auto value = solutionValue(chords, values);
        if (value < minValue) {
            solution = values;
            minValue = value;
        }
    });

    if (count == 0) {
        // Unsolvable system, revert back to placing each chord independently
        for (auto& pair : _variables) {
            auto chordGeometry = pair.first;
            if (toRemove.count(chordGeometry) == 0)
                resolve(chordGeometry);
        }
    } else {
        for (std::size_t i = 0; i < chords.size(); i += 1) {
            auto value = solution[i];
            auto chordGeometry = chords[i];
            setDirection(chordGeometry, value ? dom::Stem::Up : dom::Stem::Down);
        }
    }
}

void StemDirectionResolver::buildChords() {
    _chords.clear();

    for (auto& geometry : _measureGeometry->geometries()) {
        auto chordGeometry = dynamic_cast<ChordGeometry*>(geometry.get());
        if (!chordGeometry)
            continue;

        // Ignore chords with no stem
        if (!chordGeometry->stem())
            continue;

        _chords.push_back(chordGeometry);
    }
}

void StemDirectionResolver::buildVariables() {
    _variables.clear();
    for (auto chord : _chords) {
        addVariable(chord);
    }
}

void StemDirectionResolver::addVariable(ChordGeometry* chordGeometry) {
    auto& note = *chordGeometry->chord().firstNote();
    if (note.grace())
        return;

    const auto& voice = note.voice();
    const auto staff = note.staff();

    Variable v;
    v.chordGeometry = chordGeometry;

    for (auto chord : _chords) {
        if (chord == chordGeometry)
            continue;

        auto& n = *chord->chord().firstNote();
        if (n.voice().empty())
            continue;

        if (n.voice() == voice) {
            v.equal.insert(chord);
        } else if (n.voice() != voice && n.staff() == staff) {
            v.opposite.insert(chord);
        }
    }

    _variables.insert({chordGeometry, v});
}

void StemDirectionResolver::resolve(ChordGeometry* chordGeometry) {
    setDirection(chordGeometry, preferredStem(chordGeometry));
}

void StemDirectionResolver::setDirection(ChordGeometry* chordGeometry, dom::Stem stemDirection) {
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

    auto refLocation = chordGeometry->refNoteLocation();
    auto stem = chordGeometry->stem();

    // Set stem direction
    stem->setStemDirection(stemDirection);

    // Set stem location
    if (stemDirection == dom::Stem::Up) {
        stem->setLocation({refLocation.x, topLocation.y});
        stem->setVerticalAnchorPointValues(1, -NoteGeometry::kHeight/2);
    } else {
        stem->setLocation({refLocation.x, bottomLocation.y});
        stem->setVerticalAnchorPointValues(0, NoteGeometry::kHeight/2);
    }

    chordGeometry->setBounds(chordGeometry->subGeometriesFrame());

    // Adjust chord bounds
    if (stemDirection == dom::Stem::Up) {
        chordGeometry->setHorizontalAnchorPointValues(0, refLocation.x - chordGeometry->contentOffset().x);
        chordGeometry->setVerticalAnchorPointValues(1, -(chordGeometry->size().height - (refLocation.y - chordGeometry->contentOffset().y)));
    } else {
        chordGeometry->setHorizontalAnchorPointValues(1, -(chordGeometry->size().width - (refLocation.x - chordGeometry->contentOffset().x)));
        chordGeometry->setVerticalAnchorPointValues(0, refLocation.y - chordGeometry->contentOffset().y);
    }
}

dom::Stem StemDirectionResolver::preferredStem(const ChordGeometry* chordGeometry) const {
    auto& chord = chordGeometry->chord();
    if (chord.stem().isPresent())
        return chord.stem().value();

    auto location = chordGeometry->location();
    auto staff = chordGeometry->chord().firstNote()->staff();
    auto midStaff = _measureGeometry->metrics().staffOrigin(staff) + Metrics::staffHeight()/2;
    if (location.y <= midStaff)
        return dom::Stem::Down;
    return dom::Stem::Up;
}

int StemDirectionResolver::solutionValue(const std::vector<ChordGeometry*>& chords, const std::vector<bool>& values) const {
    // Convert values to stem directions
    for (std::size_t i = 0; i < chords.size(); i += 1) {
        auto stem = values[i] ? dom::Stem::Up : dom::Stem::Down;
        chords[i]->stem()->setStemDirection(stem);
    }

    int total = 0;
    for (auto chord : chords) {
        auto stem = chord->stem()->stemDirection();

        // Bias against high notes pointing up and low notes pointing down
        auto location = chord->location();
        auto staff = chord->chord().firstNote()->staff();
        auto midStaff = _measureGeometry->metrics().staffOrigin(staff) + Metrics::staffHeight()/2;
        if (stem == dom::Stem::Up)
            total += midStaff - location.y;
        else if (stem == dom::Stem::Down)
            total += location.y - midStaff;

        // Strong bias against 69-ing notes
        auto it = _variables.find(chord);
        for (auto c : it->second.opposite) {
            if (c->location().y < location.y && stem == dom::Stem::Up)
                total += 1000;
            else if (c->location().y > location.y && stem == dom::Stem::Down)
                total += 1000;
        }
    }

    return total;
}
} // namespace mxml
