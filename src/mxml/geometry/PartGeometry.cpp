//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "BarlineGeometry.h"
#include "ChordGeometry.h"
#include "DirectionGeometry.h"
#include "EndingGeometry.h"
#include "MeasureGeometry.h"
#include "LyricGeometry.h"
#include "OrnamentsGeometry.h"
#include "PartGeometry.h"
#include "PartGeometryFactory.h"
#include "PedalGeometry.h"
#include "SpanDirectionGeometry.h"
#include "TieGeometryFactory.h"

#include <mxml/dom/Pedal.h>
#include <mxml/dom/Wedge.h>

#include <typeindex>
#include <typeinfo>

namespace mxml {

const coord_t PartGeometry::kStaffLineSpacing = 10;
const std::size_t PartGeometry::kStaffLineCount = 5;


PartGeometry::PartGeometry(const dom::Part& part, const SpanCollection& spans) : _part(part), _spans(spans) {
    PartGeometryFactory factory(part, spans);
}

coord_t PartGeometry::noteY(const dom::Attributes& attributes, const dom::Note& note) const {
    return staffOrigin(note.staff()) + MeasureGeometry::staffY(attributes, note);
}

coord_t PartGeometry::staffHeight() {
    return (kStaffLineCount - 1) * kStaffLineSpacing;
}

coord_t PartGeometry::stavesHeight() const {
    coord_t staffDistance = _part.staffDistance();
    return (staffHeight() + staffDistance) * _part.staves() - staffDistance;
}

coord_t PartGeometry::staffOrigin(int staffNumber) const {
    return (staffNumber - 1) * (staffHeight() + _part.staffDistance());
}

} // namespace mxml
