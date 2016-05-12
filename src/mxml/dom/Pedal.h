// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "DirectionType.h"
#include "Optional.h"
#include "Types.h"

namespace mxml {
namespace dom {

class Pedal : public DirectionType {
public:
    Pedal() : _type(kStart), _line(false), _sign(true) {}

    bool span() const {
        return true;
    }

    StartStopContinue type() const {
        return _type;
    }
    void setType(StartStopContinue type) {
        _type = type;
    }

    bool line() const {
        if (_line.isPresent())
            return _line.value();
        return _sign.isPresent() && !_sign.value();
    }

    void setLine(const Optional<bool>& line) {
        _line = line;
    }

    bool sign() const {
        if (_sign.isPresent())
            return _sign;
        return !_line.isPresent() || !_line.value();
    }

    void setSign(const Optional<bool>& sign) {
        _sign = sign;
    }

private:
    StartStopContinue _type;
    Optional<bool> _line;
    Optional<bool> _sign;
};

} // namespace dom
} // namespace mxml
