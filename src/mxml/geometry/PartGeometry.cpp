//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.


#include "PartGeometry.h"
#include "PartGeometryFactory.h"

namespace mxml {

PartGeometry::PartGeometry(const dom::Part& part, const ScoreProperties& scoreProperties, const SpanCollection& spans)
: _part(part)
{
    PartGeometryFactory factory(part, scoreProperties, spans);
}

} // namespace mxml
