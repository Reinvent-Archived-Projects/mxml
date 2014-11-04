//  Created by Alejandro Isaza on 2/25/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include <lxml/IntegerHandler.h>

#include <mxml/dom/Backup.h>
#include <memory>

namespace mxml {

class BackupHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Backup>> {
public:
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    
private:
    lxml::IntegerHandler _integerHandler;
};

} // namespace mxml
