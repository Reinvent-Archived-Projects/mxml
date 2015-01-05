//  Created by Alejandro Isaza on 2015-01-05.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#pragma once
#include "PartGeometry.h"

namespace mxml {
    class LyricGeometryFactory {
    public:
        LyricGeometryFactory(PartGeometry& partGeometry);
        void build();

    protected:
        void computeNotesBounds();
        void build(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom);
        void build(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Lyric& lyric);
        
    private:
        PartGeometry& _partGeometry;
        Rect _notesBounds;
    };
}
