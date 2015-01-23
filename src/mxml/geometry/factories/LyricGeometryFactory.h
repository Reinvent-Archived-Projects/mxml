//  Created by Alejandro Isaza on 2015-01-05.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include <mxml/geometry/LyricGeometry.h>

#include <memory>
#include <vector>


namespace mxml {

class ChordGeometry;
class MeasureGeometry;

class LyricGeometryFactory {
public:
    LyricGeometryFactory(const std::vector<MeasureGeometry*>& measureGeometries, const Metrics& metrics);
    std::vector<std::unique_ptr<LyricGeometry>> build();

protected:
    void computeNotesBounds();
    void build(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom);
    void build(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Lyric& lyric);
    
private:
    const std::vector<MeasureGeometry*>& _measureGeometries;
    const Metrics& _metrics;

    std::vector<std::unique_ptr<LyricGeometry>> _lyricGeometries;
    Rect _notesBounds;
};

}
