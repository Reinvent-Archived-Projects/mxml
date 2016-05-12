// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "MeasureElementGeometry.h"
#include <mxml/dom/Note.h>

namespace mxml {

class StemGeometry : public MeasureElementGeometry {
public:
    static const coord_t kNoFlagWidth;
    static const coord_t kFlagWidth;
    static const coord_t kMinHeight;
    static const coord_t kDefaultHeight;
    static const coord_t kVerticalOffset;
    
public:
    explicit StemGeometry(const dom::Note& note, bool showFlags = true);
    
    const dom::Note& note() const {
        return _note;
    }
    
    dom::Stem stemDirection() const {
        return _stemDirection;
    }
    void setStemDirection(dom::Stem direction);

    bool showFlags() const {
        return _showFlags;
    }
    void setShowFlags(bool show);
    
    static Size Size(const dom::Note& note, dom::Stem direction, bool flags);
    
private:
    const dom::Note& _note;
    dom::Stem _stemDirection;
    bool _showFlags;
};

} // namespace mxml
