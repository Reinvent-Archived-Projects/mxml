// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "ArticulationGeometry.h"

namespace mxml {

const Size ArticulationGeometry::kAccentSize = {14, 10};
const Size ArticulationGeometry::kSpiccatoSize = {4, 8};
const Size ArticulationGeometry::kStaccatoSize = {3, 3};
const Size ArticulationGeometry::kStaccatissimoSize = {6, 9};

ArticulationGeometry::ArticulationGeometry(const dom::Articulation& articulation, dom::Stem stem) : _articulation(articulation), _stem(stem) {
    if (articulation.type() == dom::Articulation::Type::Staccato) {
        setSize(kStaccatoSize);
    } else if (articulation.type() == dom::Articulation::Type::Accent) {
        setSize(kAccentSize);
    } else if (articulation.type() == dom::Articulation::Type::Spiccato) {
        setSize(kSpiccatoSize);
    } else if (articulation.type() == dom::Articulation::Type::Staccatissimo) {
        setSize(kStaccatissimoSize);
    }
}

} // namespace mxml
