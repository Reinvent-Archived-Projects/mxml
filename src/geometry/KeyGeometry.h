//  Created by Alejandro Isaza on 2014-05-13.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "MeasureElementGeometry.h"
#include "dom/Clef.h"
#include "dom/Key.h"

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
    
    static Size keySize(const dom::Key& key);
    
    /**
     Return the y position of an accidental in a key signature relative to the staff.
     */
    static coord_t keyStaffY(int fifth, const dom::Clef& clef);
    
private:
    const dom::Key& _key;
    const dom::Clef& _clef;
};

} // namespace mxml
