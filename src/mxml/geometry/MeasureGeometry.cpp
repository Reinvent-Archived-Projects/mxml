// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "MeasureGeometry.h"

#include "BarlineGeometry.h"
#include "BeamGeometry.h"
#include "ChordGeometry.h"
#include "ClefGeometry.h"
#include "KeyGeometry.h"
#include "NoteGeometry.h"
#include "RestGeometry.h"
#include "TimeSignatureGeometry.h"
#include "factories/StemDirectionResolver.h"

#include <mxml/dom/Backup.h>
#include <mxml/dom/Forward.h>
#include <mxml/Metrics.h>

namespace mxml {

using namespace dom;

const coord_t MeasureGeometry::kGraceNoteScale = 0.85;

MeasureGeometry::MeasureGeometry(const Measure& measure,
                                 const SpanCollection& spans,
                                 const ScoreProperties& scoreProperties,
                                 const Metrics& metrics)
: _measure(measure),
  _spans(spans),
  _scoreProperties(scoreProperties),
  _metrics(metrics),
  _partIndex(metrics.partIndex()),
  _showNumber(false)
{
}

} // namespace mxml
