// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Geometry.h"
#include <mxml/Metrics.h>
#include <mxml/dom/Barline.h>

namespace mxml {

class BarlineGeometry : public Geometry {
public:
    static const coord_t kLightLineWidth;
    static const coord_t kHeavyLineWidth;
    static const coord_t kLineSpacing;
    static const coord_t kDotDiameter;
    
public:
    BarlineGeometry(const dom::Barline& barline, const Metrics& metrics);
    
    const dom::Barline& barline() const {
        return _barline;
    }
    const Metrics& metrics() const {
        return _metrics;
    }
    
    static coord_t Width(const dom::Barline& barline);
    
private:
    const dom::Barline& _barline;
    const Metrics& _metrics;
};

} // namespace mxml
