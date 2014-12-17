//  Created by Alejandro Isaza on 2/20/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include <lxml/BaseRecursiveHandler.h>
#include "AttributesHandler.h"
#include "BackupHandler.h"
#include "BarlineHandler.h"
#include "DirectionHandler.h"
#include "ForwardHandler.h"
#include "NoteHandler.h"
#include "PrintHandler.h"

#include <mxml/dom/Chord.h>
#include <mxml/dom/Measure.h>
#include <memory>

namespace mxml {

class MeasureHandler : public lxml::BaseRecursiveHandler<std::unique_ptr<dom::Measure>> {
public:
    MeasureHandler() : _lastTime(), _time() {}
    
    RecursiveHandler* startSubElement(const lxml::QName& qname);
    void endElement(const lxml::QName& qname, const std::string& contents);

    void endSubElement(const lxml::QName& qname, RecursiveHandler* parser);
    void startElement(const lxml::QName& qname, const AttributeMap& attributes);
    
private:
    void handleNote(std::unique_ptr<dom::Note>&& note);
    void startChord(std::unique_ptr<dom::Note>&& note);
    void endChord();
    
private:
    AttributesHandler _attributesHandler;
    NoteHandler _noteHandler;
    BackupHandler _backupHandler;
    ForwardHandler _forwardHandler;
    DirectionHandler _directionHandler;
    PrintHandler _printHandler;
    BarlineHandler _barlineHandler;

    const dom::Attributes* _attributes;
    dom::Chord* _chord;
    int _lastTime;
    int _time;
    bool _empty;
};

} // namespace mxml
