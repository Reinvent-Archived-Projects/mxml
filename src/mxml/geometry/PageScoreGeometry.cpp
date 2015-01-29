//  Created by Alejandro Isaza on 2015-01-23.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

#include "PageScoreGeometry.h"


namespace mxml {

PageScoreGeometry::PageScoreGeometry(const dom::Score& score, coord_t width)
: _score(score),
  _scoreProperties(score)
{
    coord_t offset = 0;
    for (std::size_t systemIndex = 0; systemIndex < _scoreProperties.systemCount(); systemIndex += 1) {
        auto systemGeometry = std::unique_ptr<SystemGeometry>(new SystemGeometry(_score, _scoreProperties, systemIndex, width));
        auto& metrics = systemGeometry->metrics(_score.parts().size() - 1);

        systemGeometry->setHorizontalAnchorPointValues(0, 0);
        systemGeometry->setVerticalAnchorPointValues(0, 0);
        systemGeometry->setLocation({0, offset});

        auto systemDistance = metrics.systemDistance();
        offset += systemGeometry->size().height + systemDistance - 2*MeasureGeometry::kVerticalPadding;

        _systemGeometries.push_back(systemGeometry.get());
        addGeometry(std::move(systemGeometry));
    }
    setBounds(subGeometriesFrame());

    // Force the width even if there are systems that didn't fit
    auto s = size();
    s.width = width;
    setSize(s);
}

} // namespace mxml
