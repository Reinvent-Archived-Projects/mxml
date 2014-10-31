//  Created by Alejandro Isaza on 2014-05-08.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "ArticulationGeometry.h"

namespace mxml {

const Size ArticulationGeometry::kAccentSize = {14, 10};
const Size ArticulationGeometry::kSpiccatoSize = {4, 8};
const Size ArticulationGeometry::kStaccatoSize = {3, 3};
const Size ArticulationGeometry::kStaccatissimoSize = {6, 9};

ArticulationGeometry::ArticulationGeometry(const dom::Articulation& articulation, dom::Stem stem) : _articulation(articulation), _stem(stem) {
    if (articulation.type() == dom::Articulation::STACCATO) {
        setSize(kStaccatoSize);
    } else if (articulation.type() == dom::Articulation::ACCENT) {
        setSize(kAccentSize);
    } else if (articulation.type() == dom::Articulation::SPICCATO) {
        setSize(kSpiccatoSize);
    } else if (articulation.type() == dom::Articulation::STACCATISSIMO) {
        setSize(kStaccatissimoSize);
    }
}

} // namespace mxml
