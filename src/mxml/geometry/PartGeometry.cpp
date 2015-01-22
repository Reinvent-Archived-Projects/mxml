//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "PartGeometry.h"


namespace mxml {

PartGeometry::PartGeometry(const dom::Part& part, const ScoreProperties& scoreProperties, const ScrollMetrics& metrics, const SpanCollection& spans)
: _part(part),
  _scoreProperties(scoreProperties),
  _metrics(metrics)
{
    _staves = scoreProperties.staves(part.index());
    _staffDistance = metrics.staffDistance();
}

dom::tenths_t PartGeometry::noteY(const dom::Note& note) const {
    return staffOrigin(note.staff()) + _metrics.staffY(note);
}
    
} // namespace mxml
