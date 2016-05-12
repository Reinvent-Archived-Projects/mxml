// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#include "LoopFactory.h"

#include <mxml/dom/Barline.h>
#include <cassert>


namespace mxml {

LoopFactory::LoopFactory(const dom::Score& score) : _score(score) {
}

std::vector<Loop> LoopFactory::build() {
    _loops.clear();
    _loops.emplace_back();
    _endingBegin = 0;

    for (auto& part : _score.parts()) {
        std::size_t measureIndex = 0;
        for (auto& measure : part->measures()) {
            assert(measure->index() == measureIndex);
            for (auto& node : measure->nodes()) {
                if (const dom::Barline* barline = dynamic_cast<const dom::Barline*>(node.get()))
                    process(measureIndex, *barline);
            }
            measureIndex += 1;
        }
    }

    return _loops;
}

void LoopFactory::process(std::size_t measureIndex, const dom::Barline& barline) {
    Loop* loop = &_loops.back();

    if (barline.repeat()) {
        const auto& repeat = barline.repeat();
        if (repeat->direction() == dom::Repeat::Direction::Forward) {
            if (loop->begin() != loop->end()) {
                _loops.emplace_back();
                loop = &_loops.back();
            }
            loop->setBegin(measureIndex);
            loop->setEnd(measureIndex);
            _endingBegin = measureIndex;
        } else {
            loop->setEnd(measureIndex + 1);
        }
    }

    if (barline.ending()) {
        const auto& ending = barline.ending();
        if (!ending->numbers().empty()) {
            std::size_t maxNumber = *ending->numbers().rbegin();
            if (maxNumber > 0)
                maxNumber -= 1;
            loop->setCount(std::max(loop->count(), maxNumber));
        }

        if (ending->type() == dom::Ending::Type::Start) {
            _endingBegin = measureIndex;
        } else {
            for (std::size_t i = 0; i <= loop->count(); i += 1) {
                if (ending->numbers().count(static_cast<int>(i + 1)) == 0)
                    loop->skip(i, _endingBegin, measureIndex + 1);
            }
        }
    }
}

}
