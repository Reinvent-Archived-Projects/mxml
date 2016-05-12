// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "MeasureElementGeometry.h"
#include <mxml/dom/Note.h>

namespace mxml {
    
class TieGeometry;

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
    
    const TieGeometry* tieGeometry() const;
    void setTieGeometry(const TieGeometry* tieGeometry);
    
private:
    const dom::Note& _note;
    const TieGeometry *_tieGeometry;
};

} // namespace mxml
