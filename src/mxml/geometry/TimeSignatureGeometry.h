//  Created by Alejandro Isaza on 2014-05-05.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "MeasureElementGeometry.h"
#include <mxml/dom/Time.h>

namespace mxml {

class TimeSignatureGeometry : public MeasureElementGeometry {
public:
    static const coord_t kVerticalSpacing;
    static const coord_t kHorizontalSpacing;
    static const Size kDigitSize;
    static const Size kCommonSize;
    
public:
    explicit TimeSignatureGeometry(const dom::Time& time);
    
    const dom::Time& time() const {
        return _time;
    }
    
    static Size Size(const dom::Time& time);

private:
    static int numberOfDigits(int number);
    
private:
    const dom::Time& _time;
};

} // namespace mxml
