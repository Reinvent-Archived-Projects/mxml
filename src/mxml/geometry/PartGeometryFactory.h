//  Created by Alejandro Isaza on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "PartGeometry.h"

#include <mxml/AttributesManager.h>
#include <mxml/dom/Part.h>

namespace mxml {
    class PartGeometryFactory {
    public:
        PartGeometryFactory(const dom::Part& part, const SpanCollection& spans);
        std::unique_ptr<PartGeometry> build();

    private:
        void buildDirections();
        void buildDirection(const MeasureGeometry&  measureGeom, const dom::Direction& direction);
        void buildWedge(const MeasureGeometry& measureGeom, const dom::Direction& direction);
        void buildWedge(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                        const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);
        void buildPedal(const MeasureGeometry& measureGeom, const dom::Direction& direction);
        void buildPedal(const MeasureGeometry& startMeasureGeom, const dom::Direction& startDirection,
                        const MeasureGeometry& stopMeasureGeom, const dom::Direction& stopDirection);

        void buildOrnaments();
        void buildOrnaments(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom);
        void buildOrnament(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Ornaments& ornaments);

        void buildEndings();
        void buildEndings(const MeasureGeometry& measureGeom, const BarlineGeometry& barlineGeom);

        void buildLyrics();
        void buildLyrics(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom);
        void buildLyric(const MeasureGeometry& measureGeom, const ChordGeometry& chordGeom, const dom::Lyric& lyric);

        void placeDirection(PlacementGeometry& geometry);
        void swapPlacement(PlacementGeometry& geometry);

    private:
        const dom::Part& _part;
        const SpanCollection& _spans;

        std::unique_ptr<PartGeometry> _partGeometry;
        std::vector<std::pair<const MeasureGeometry*, const dom::Direction*>> _openSpanDirections;

        dom::Optional<dom::Ending> _startEnding;
        Point _startEndingLocation;
        
        AttributesManager _attributesManager;
    };
}
