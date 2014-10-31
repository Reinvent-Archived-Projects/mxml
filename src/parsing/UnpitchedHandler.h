//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "BaseRecursiveHandler.h"
#include "IntegerHandler.h"
#include "StringHandler.h"

#include "dom/Unpitched.h"

namespace mxml {

class UnpitchedHandler : public lxml::BaseRecursiveHandler<dom::Unpitched> {
public:
    virtual RecursiveHandler* startSubElement(const lxml::QName& qname);
    virtual void endSubElement(const lxml::QName& qname, lxml::RecursiveHandler* parser);
    
private:
    lxml::IntegerHandler _integerHandler;
    lxml::StringHandler _stringHandler;
};

} // namespace mxml
