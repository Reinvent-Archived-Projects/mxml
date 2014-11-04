//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Geometry.h"
#include "PartGeometry.h"
#include <mxml/dom/Barline.h>

namespace mxml {

class BarlineGeometry : public Geometry {
public:
    static const coord_t kLightLineWidth;
    static const coord_t kHeavyLineWidth;
    static const coord_t kLineSpacing;
    static const coord_t kDotDiameter;
    
public:
    BarlineGeometry(const dom::Barline& barline, const PartGeometry& partGeometry);
    
    const dom::Barline& barline() const {
        return _barline;
    }
    const PartGeometry& partGeometry() const {
        return _partGeometry;
    }
    
    static coord_t Width(const dom::Barline& barline);
    
private:
    const dom::Barline& _barline;
    const PartGeometry& _partGeometry;
};

} // namespace mxml
