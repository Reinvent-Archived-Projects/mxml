//  Created by Alejandro Isaza on 2014-05-27.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "Part.h"
#include "Print.h"
#include "Score.h"

namespace mxml {
namespace dom {

const Measure* Part::measureWithNumber(const std::string& number) const {
    auto it = std::find_if(_measures.begin(), _measures.end(), [number](const std::unique_ptr<Measure>& measure) {
        return measure->number() == number;
    });
    if (it == _measures.end())
        return nullptr;
    return it->get();
}

std::size_t Part::indexOfMeasure(const Measure* measure) const {
    auto it = std::find_if(_measures.begin(), _measures.end(), [measure](const std::unique_ptr<Measure>& m) {
        return m.get() == measure;
    });
    if (it == _measures.end())
        return static_cast<std::size_t>(-1);
    return it - _measures.begin();
}

float Part::staffDistance() const {
    float defaultStaffDistance = 65;
    
    const Score* score = dynamic_cast<const Score*>(parent());
    if (score)
        defaultStaffDistance = score->defaults().staffLayout().value().staffDistance();
    
    if (_measures.empty())
        return defaultStaffDistance;
    auto& measure = _measures.front();
    
    const Print* print = 0;
    for (auto& node : measure->nodes()) {
        print = dynamic_cast<const Print*>(node.get());
        if (print)
            break;
    }
    if (!print || !print->staffLayout().isPresent())
        return defaultStaffDistance;
    
    const StaffLayout& staffLayout = print->staffLayout();
    if (!staffLayout.staffDistance().isPresent())
        return defaultStaffDistance;
    
    return staffLayout.staffDistance().value();
}

int Part::staves() const {
    if (_measures.empty())
        return 0;
    auto& measure = _measures.front();
    
    const Attributes* attrs = 0;
    for (auto& node : measure->nodes()) {
        attrs = dynamic_cast<const Attributes*>(node.get());
        if (attrs)
            break;
    }
    
    if (!attrs)
        return 0;
    
    return attrs->staves();
}

} // namespace dom
} // namespace mxml
