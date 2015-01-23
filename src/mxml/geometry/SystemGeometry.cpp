//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "SystemGeometry.h"

namespace mxml {

SystemGeometry::SystemGeometry(const dom::Part& part, const System& system, const ScoreProperties& scoreProperties, const PageMetrics& metrics)
: _part(part),
  _system(system),
  _scoreProperties(scoreProperties),
  _metrics(metrics)
{
}

} // namespace
