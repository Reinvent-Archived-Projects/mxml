// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "MeasureElementGeometry.h"
#include <mxml/dom/Clef.h>
#include <mxml/dom/Key.h>

namespace mxml {

class KeyGeometry : public MeasureElementGeometry {
public:
    static const coord_t kSpacing;
    
public:
    explicit KeyGeometry(const dom::Key& key, const dom::Clef& clef);
    
    const dom::Key& key() const {
        return _key;
    }
    const dom::Clef& clef() const {
        return _clef;
    }
    
    void setNatural(const bool natural) {
        _natural = natural;
    }
    bool natural() const {
        return _natural;
    }
    
    static Size keySize(const dom::Key& key);
    
    /**
     Return the y position of an accidental in a key signature relative to the staff.
     */
    static coord_t keyStaffY(int fifth, const dom::Clef& clef);
    
    
private:
    const dom::Key& _key;
    const dom::Clef& _clef;
    bool _natural;
};

} // namespace mxml
