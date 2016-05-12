// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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
