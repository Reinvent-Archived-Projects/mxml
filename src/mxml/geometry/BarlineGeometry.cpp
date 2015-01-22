//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BarlineGeometry.h"

#include <mxml/Metrics.h>

namespace mxml {

using dom::Barline;

const coord_t BarlineGeometry::kLightLineWidth = 1;
const coord_t BarlineGeometry::kHeavyLineWidth = 2;
const coord_t BarlineGeometry::kLineSpacing = 2;
const coord_t BarlineGeometry::kDotDiameter = 4;

BarlineGeometry::BarlineGeometry(const Barline& barline, const Metrics& metrics) : _barline(barline), _metrics(metrics) {
    Size size;
    size.width = Width(_barline);
    size.height = metrics.stavesHeight();
    if (_barline.style() == Barline::TICK)
        size.height = Metrics::kStaffLineSpacing;
    else if (_barline.style() == Barline::SHORT)
        size.height = 2*Metrics::kStaffLineSpacing;
    setSize(size);
}

coord_t BarlineGeometry::Width(const Barline& barline) {
    if (barline.repeat())
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
