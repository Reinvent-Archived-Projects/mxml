// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "MeasureElementGeometry.h"
#include <mxml/dom/Note.h>

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
