// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "EmptyPlacement.h"
#include "Mordent.h"
#include "Node.h"
#include "Turn.h"

#include <memory>


namespace mxml {
namespace dom {

class Ornaments : public Node {
public:
    const EmptyPlacement* placement() const {
        if (_trillMark.get())
            return _trillMark.get();
        if (_mordent.get())
            return _mordent.get();
        if (_invertedMordent.get())
            return _invertedMordent.get();
        if (_turn.get())
            return _turn.get();
        if (_invertedTurn.get())
            return _invertedTurn.get();
        
        return nullptr;
    }
    
    const std::unique_ptr<EmptyPlacement>& trillMark() const {
        return _trillMark;
    }
    void setTrillMark(std::unique_ptr<EmptyPlacement>&& trillMark) {
        _trillMark = std::move(trillMark);
    }

    const std::unique_ptr<Mordent>& mordent() const {
        return _mordent;
    }
    void setMordent(std::unique_ptr<Mordent>&& mordent) {
        _mordent = std::move(mordent);
    }

    const std::unique_ptr<Mordent>& invertedMordent() const {
        return _invertedMordent;
    }
    void setInvertedMordent(std::unique_ptr<Mordent>&& mordent) {
        _invertedMordent = std::move(mordent);
    }

    const std::unique_ptr<Turn>& turn() const {
        return _turn;
    }
    void setTurn(std::unique_ptr<Turn>&& turn) {
        _turn = std::move(turn);
    }

    const std::unique_ptr<Turn>& invertedTurn() const {
        return _invertedTurn;
    }
    void setInvertedTurn(std::unique_ptr<Turn>&& turn) {
        _invertedTurn = std::move(turn);
    }

private:
    std::unique_ptr<EmptyPlacement> _trillMark;
    std::unique_ptr<Mordent> _mordent;
    std::unique_ptr<Mordent> _invertedMordent;
    std::unique_ptr<Turn> _turn;
    std::unique_ptr<Turn> _invertedTurn;
};

} // namespace dom
} // namespace mxml
