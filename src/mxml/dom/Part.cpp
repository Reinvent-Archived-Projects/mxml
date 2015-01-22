//  Created by Alejandro Isaza on 2014-05-27.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "Part.h"


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

} // namespace dom
} // namespace mxml
