//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "MeasureElementGeometry.h"
#include "dom/Note.h"

namespace mxml {

class NoteGeometry : public MeasureElementGeometry {
public:
    static const coord_t kHeight;
    static const coord_t kQuarterWidth;
    static const coord_t kWholeWidth;
    
public:
    explicit NoteGeometry(const dom::Note& note);
    
    const dom::Note& note() const {
        return _note;
    }
    
    static Size Size(const dom::Note& note);
    
private:
    const dom::Note& _note;
};

} // namespace mxml
