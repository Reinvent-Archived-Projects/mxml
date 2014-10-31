//  Created by Alejandro Isaza on 2014-05-06.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "MeasureElementGeometry.h"
#include "dom/Note.h"

namespace mxml {

class RestGeometry : public MeasureElementGeometry {
public:
    static const Size k128thSize;
    static const Size k64thSize;
    static const Size k32thSize;
    static const Size k16thSize;
    static const Size kEighthSize;
    static const Size kQuarterSize;
    static const Size kHalfSize;
    static const Size kWholeSize;
    static const Size kBreveSize;
    static const Size kLongSize;
    static const Size kMaximaSize;
    
public:
    explicit RestGeometry(const dom::Note& note);
    
    const dom::Note& note() const {
        return _note;
    }
    
    static Size Size(const dom::Note& note);
    
private:
    const dom::Note& _note;
};

} // namespace mxml
