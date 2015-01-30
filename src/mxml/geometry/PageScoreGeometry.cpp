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

        const auto systemDistance = metrics.systemDistance();
        const auto bottomPadding = systemGeometry->bottomPadding();
        const auto topPadding = systemGeometry->topPadding();
        if (offset == 0)
            offset = topPadding;

        systemGeometry->setHorizontalAnchorPointValues(0, 0);
        systemGeometry->setVerticalAnchorPointValues(0, 0);
        systemGeometry->setLocation({0, offset - topPadding});

        offset += systemGeometry->size().height - topPadding - bottomPadding + systemDistance;

        _systemGeometries.push_back(systemGeometry.get());
        addGeometry(std::move(systemGeometry));
    }

    // Force the content offset in x and width so that the page aligns to the screen
    auto bounds = subGeometriesFrame();
    bounds.origin.x = 0;
    bounds.size.width = width;
    setBounds(bounds);
}

} // namespace mxml
