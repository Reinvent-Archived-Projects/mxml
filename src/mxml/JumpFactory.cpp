// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "JumpFactory.h"

#include <mxml/dom/Direction.h>


namespace mxml {

JumpFactory::JumpFactory(const dom::Score& score) : _score(score) {
}

std::vector<Jump> JumpFactory::build() {
    for (auto& part : _score.parts()) {
        std::size_t measureIndex = 0;
        for (auto& measure : part->measures()) {
            for (auto& node : measure->nodes()) {
                if (auto direction = dynamic_cast<const dom::Direction*>(node.get())) {
                    process(measureIndex, *direction);
                }
            }
            measureIndex += 1;
        }
    }

    std::vector<Jump> jumps;
    for (auto it : _jumps) {
        auto& jump = it.second;
        if (jump.from != jump.to)
            jumps.push_back(jump);
    }
    return jumps;
}

void JumpFactory::process(std::size_t measureIndex, const dom::Direction& direction) {
    auto& sound = direction.sound();
    if (!sound)
        return;

    if (sound->segno.isPresent()) {
        auto& name = sound->segno.value();
        Jump jump;
        jump.from = measureIndex;
        jump.to = measureIndex;
        _jumps[name] = jump;
    } else if (sound->dalsegno.isPresent()) {
        auto& name = sound->dalsegno.value();
        auto it = _jumps.find(name);
        if (it != _jumps.end()) {
            auto& jump = it->second;
            jump.from = measureIndex;
        }
    } else if (sound->tocoda.isPresent()) {
        auto& name = sound->tocoda.value();
        Jump jump;
        jump.from = measureIndex;
        jump.to = measureIndex;
        _jumps[name] = jump;
    } else if (sound->coda.isPresent()) {
        auto& name = sound->coda.value();
        auto it = _jumps.find(name);
        if (it != _jumps.end()) {
            auto& jump = it->second;
            jump.to = measureIndex;
        }
    }
}

}
