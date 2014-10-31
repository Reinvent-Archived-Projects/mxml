//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Articulation.h"
#include "Fermata.h"
#include "Node.h"
#include "Optional.h"
#include "Ornaments.h"
#include "Slur.h"
#include "Tied.h"

#include <vector>

namespace mxml {
namespace dom {

class Notations : public Node {
public:
    Notations() : _printObject(true) {}
    
    bool printObject() const {
        return _printObject;
    }
    void setPrintObject(bool printObject) {
        _printObject = printObject;
    }
    
    const std::vector<Articulation>& articulations() const {
        return _articulations;
    }
    void setArticulations(const std::vector<Articulation>& articulations) {
        _articulations = articulations;
    }
    void addArticulation(const Articulation& articulation) {
        _articulations.push_back(articulation);
    }
    
    const Optional<Fermata>& fermata() const {
        return _fermata;
    }
    void setFermata(const Optional<Fermata>& fermata) {
        _fermata = fermata;
    }
    
    const std::vector<Slur>& slurs() const {
        return _slurs;
    }
    void addSlur(const Slur& slur) {
        _slurs.push_back(slur);
    }
    
    const std::vector<Tied>& ties() const {
        return _ties;
    }
    void addTied(const Tied& tied) {
        _ties.push_back(tied);
    }

    const std::vector<Ornaments>& ornaments() const {
        return _ornaments;
    }
    void addOrnaments(const Ornaments& ornaments) {
        _ornaments.push_back(ornaments);
    }
    
private:
    bool _printObject;
    Optional<Fermata> _fermata;
    std::vector<Articulation> _articulations;
    std::vector<Slur> _slurs;
    std::vector<Tied> _ties;
    std::vector<Ornaments> _ornaments;
};

} // namespace dom
} // namespace mxml
