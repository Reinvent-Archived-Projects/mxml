//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

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
    
    bool showFlags() const {
        return _showFlags;
    }
    void setShowFlags(bool show);
    
    static Size Size(const dom::Note& note, bool flags);
    
private:
    const dom::Note& _note;
    bool _showFlags;
};

} // namespace mxml
