//  Created by Alejandro Isaza on 2014-03-25.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Node.h"

#include <string>

namespace mxml {
namespace dom {

class CreditWords : public Node {
public:
    enum Justify {
        JUSTIFY_LEFT,
        JUSTIFY_CENTER,
        JUSTIFY_RIGHT
    };
    
    enum FontStyle {
        FONT_STYLE_NORMAL,
        FONT_STYLE_ITALIC
    };
    
    enum FontWeight {
        FONT_WEIGHT_NORMAL,
        FONT_WEIGHT_BOLD
    };
    
public:
    CreditWords() {}
    
    float defaultX() const {
        return _defaultX;
    }
    void setDefaultX(float x) {
        _defaultX = x;
    }
    
    float defaultY() const {
        return _defaultY;
    }
    void setDefaultY(float y) {
        _defaultY = y;
    }
    
    Justify justify() const {
        return _justify;
    }
    void setJustify(Justify justify) {
        _justify = justify;
    }
    
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
    
    const std::string& contents() const {
        return _contents;
    }
    void setContents(const std::string& contents) {
        _contents = contents;
    }
    
private:
    float _defaultX;
    float _defaultY;
    
    Justify _justify;
    std::string _fontFamily;
    FontStyle _fontStyle;
    FontWeight _fontWeight;
    float _fontSize;
    
    std::string _contents;
};

} // namespace dom
} // namespace mxml
