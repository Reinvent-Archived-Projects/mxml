// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Loop.h"

#include <mxml/dom/Score.h>

#include <map>
#include <set>
#include <string>


namespace mxml {
namespace dom {
    class Barline;
}

class LoopFactory {
public:
    LoopFactory(const dom::Score& score);
    std::vector<Loop> build();

protected:
    void process(std::size_t measureIndex, const dom::Barline& barline);
    
private:
    const dom::Score& _score;
    std::vector<Loop> _loops;
    std::size_t _endingBegin;
};

}
