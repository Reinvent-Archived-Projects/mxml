// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
    if (_barline.style() == Barline::Style::Tick)
        size.height = Metrics::kStaffLineSpacing;
    else if (_barline.style() == Barline::Style::Short)
        size.height = 2*Metrics::kStaffLineSpacing;
    setSize(size);
}

coord_t BarlineGeometry::Width(const Barline& barline) {
    if (barline.repeat())
        return kLightLineWidth + kLineSpacing + kHeavyLineWidth + kLineSpacing + kDotDiameter;
    
    switch (barline.style()) {
        case Barline::Style::Regular:
        case Barline::Style::Dotted:
        case Barline::Style::Dashed:
            return kLightLineWidth;
            
        case Barline::Style::Heavy:
            return kHeavyLineWidth;
            
        case Barline::Style::LightLight:
            return kLightLineWidth + kLineSpacing + kLightLineWidth;
            
        case Barline::Style::LightHeavy:
        case Barline::Style::HeavyLight:
            return kLightLineWidth + kLineSpacing + kHeavyLineWidth;
            
        case Barline::Style::HeavyHeavy:
            return kHeavyLineWidth + kLineSpacing + kHeavyLineWidth;
            
        case Barline::Style::Tick:
            return kLightLineWidth;
            
        case Barline::Style::Short:
            return kLightLineWidth;
            
        case Barline::Style::None:
            return kLightLineWidth;
    }
}

} // namespace mxml
