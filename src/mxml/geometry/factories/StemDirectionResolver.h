// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/geometry/ChordGeometry.h>
#include <mxml/geometry/MeasureGeometry.h>

#include <unordered_map>
#include <unordered_set>
#include <vector>


namespace mxml {

class StemDirectionResolver {
public:
    using ChordSet = std::unordered_set<ChordGeometry*>;
    
public:
    StemDirectionResolver();
    void resolve(const MeasureGeometry* measureGeometry);
    void resolve(ChordGeometry* chordGeometry);
    void setDirection(ChordGeometry* chordGeometry, dom::Stem stem);

    dom::Stem preferredStem(const ChordGeometry* chordGeometry) const;

private:
    struct Variable {
        ChordGeometry* chordGeometry;
        ChordSet equal;
        ChordSet opposite;
    };

private:
    void buildChords();
    void buildVariables();
    void addVariable(ChordGeometry* chordGeometry);

    int solutionValue(const std::vector<ChordGeometry*>& chords, const std::vector<bool>& values) const;

private:
    std::vector<ChordGeometry*> _chords;
    std::unordered_map<ChordGeometry*, Variable> _variables;

    const MeasureGeometry* _measureGeometry;
};

} // namespace mxml
