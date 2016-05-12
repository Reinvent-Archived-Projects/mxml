// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include <mxml/geometry/LyricGeometry.h>

#include <memory>
#include <vector>


namespace mxml {

class ChordGeometry;
class MeasureGeometry;

class LyricGeometryFactory {
public:
    LyricGeometryFactory(const Geometry& parent, const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics);
    std::vector<std::unique_ptr<LyricGeometry>> build();

protected:
    void computeNotesBounds();
    void build(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom);
    void build(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Lyric& lyric);
    
private:
    const Geometry& _parent;
    const std::vector<MeasureGeometry*>& _measureGeometries;
    const Metrics& _metrics;

    std::vector<std::unique_ptr<LyricGeometry>> _lyricGeometries;
    std::vector<Rect> _chordsBounds;
};

}
