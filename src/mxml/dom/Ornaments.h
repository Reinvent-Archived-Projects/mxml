//  Created by Alejandro Isaza on 2014-06-03.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "EmptyPlacement.h"
#include "Mordent.h"
#include "Node.h"
#include "Optional.h"
#include "Turn.h"

namespace mxml {
namespace dom {

class Ornaments : public Node {
public:
    const EmptyPlacement* placement() const {
        if (_trillMark.isPresent())
            return &_trillMark.value();
        if (_mordent.isPresent())
            return &_mordent.value();
        if (_invertedMordent.isPresent())
            return &_invertedMordent.value();
        if (_turn.isPresent())
            return &_turn.value();
        if (_invertedTurn.isPresent())
            return &_turn.value();
        return nullptr;
    }
    
    const Optional<EmptyPlacement>& trillMark() const {
        return _trillMark;
    }
    void setTrillMark(const Optional<EmptyPlacement>& trillMark) {
        _trillMark = trillMark;
    }

    const Optional<Mordent>& mordent() const {
        return _mordent;
    }
    void setMordent(const Optional<Mordent>& mordent) {
        _mordent = mordent;
    }

    const Optional<Mordent>& invertedMordent() const {
        return _invertedMordent;
    }
    void setInvertedMordent(const Optional<Mordent>& mordent) {
        _invertedMordent = mordent;
    }

    const Optional<Turn>& turn() const {
        return _turn;
    }
    void setTurn(const Optional<Turn>& turn) {
        _turn = turn;
    }

    const Optional<Turn>& invertedTurn() const {
        return _invertedTurn;
    }
    void setInvertedTurn(const Optional<Turn>& turn) {
        _invertedTurn = turn;
    }

private:
    Optional<EmptyPlacement> _trillMark;
    Optional<Mordent> _mordent;
    Optional<Mordent> _invertedMordent;
    Optional<Turn> _turn;
    Optional<Turn> _invertedTurn;
};

} // namespace dom
} // namespace mxml
