//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BarlineGeometry.h"

namespace mxml {

using dom::Barline;

const coord_t BarlineGeometry::kLightLineWidth = 1;
const coord_t BarlineGeometry::kHeavyLineWidth = 2;
const coord_t BarlineGeometry::kLineSpacing = 2;
const coord_t BarlineGeometry::kDotDiameter = 4;

BarlineGeometry::BarlineGeometry(const Barline& barline, const PartGeometry& partGeometry) : _barline(barline), _partGeometry(partGeometry) {
    Size size;
    size.width = Width(_barline);
    size.height = _partGeometry.stavesHeight();
    if (_barline.style() == Barline::TICK)
        size.height = PartGeometry::kStaffLineSpacing;
    else if (_barline.style() == Barline::SHORT)
        size.height = 2*PartGeometry::kStaffLineSpacing;
    setSize(size);
}

coord_t BarlineGeometry::Width(const Barline& barline) {
    if (barline.repeat().isPresent())
        return kLightLineWidth + kLineSpacing + kHeavyLineWidth + kLineSpacing + kDotDiameter;
    
    switch (barline.style()) {
        case Barline::REGULAR:
        case Barline::DOTTED:
        case Barline::DASHED:
            return kLightLineWidth;
            
        case Barline::HEAVY:
            return kHeavyLineWidth;
            
        case Barline::LIGHT_LIGHT:
            return kLightLineWidth + kLineSpacing + kLightLineWidth;
            
        case Barline::LIGHT_HEAVY:
        case Barline::HEAVY_LIGHT:
            return kLightLineWidth + kLineSpacing + kHeavyLineWidth;
            
        case Barline::HEAVY_HEAVY:
            return kHeavyLineWidth + kLineSpacing + kHeavyLineWidth;
            
        case Barline::TICK:
            return kLightLineWidth;
            
        case Barline::SHORT:
            return kLightLineWidth;
            
        case Barline::NONE:
            return kLightLineWidth;
    }
}

} // namespace mxml
