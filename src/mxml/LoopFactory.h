//  Created by Alejandro Isaza on 2015-01-14.
//  Copyright (c) 2015 Venture Media Labs. All rights reserved.

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
