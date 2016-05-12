// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Position.h"
#include "Types.h"

#include <string>

namespace mxml {
namespace dom {

class CreditWords : public Node {
public:
    enum class FontStyle {
        Normal,
        Italic
    };
    
    enum class FontWeight {
        Normal,
        Bold
    };
    
public:
    CreditWords() {}

    const std::string& fontFamily() const {
        return _fontFamily;
    }
    void setFontFamily(const std::string& fontFamily) {
        _fontFamily = fontFamily;
    }
    
    FontStyle fontStyle() const {
        return _fontStyle;
    }
    void setFontStyle(FontStyle style) {
        _fontStyle = style;
    }
    
    FontWeight fontWeight() const {
        return _fontWeight;
    }
    void setFontWeight(FontWeight weight) {
        _fontWeight = weight;
    }
    
    float fontSize() const {
        return _fontSize;
    }
    void setFontSize(float size) {
        _fontSize = size;
    }

public:
    Justify justify;
    Position position;

    std::string contents;

private:
    std::string _fontFamily;
    FontStyle _fontStyle;
    FontWeight _fontWeight;
    float _fontSize;
};

} // namespace dom
} // namespace mxml
