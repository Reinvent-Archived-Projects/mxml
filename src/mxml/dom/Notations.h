//  Created by Alejandro Isaza on 2014-04-23.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#pragma once
#include "Articulation.h"
#include "Fermata.h"
#include "Node.h"
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
   
    void addArticulation(std::unique_ptr<Articulation> articulation) {
        _articulations.push_back(std::move(articulation));
    }
    
    const std::vector<std::unique_ptr<Articulation>>& articulations() const {
        return _articulations;
    }
    void setArticulations(std::vector<std::unique_ptr<Articulation>> articulations) {
        _articulations = std::move(articulations);
    }
    
    const std::unique_ptr<Fermata>& fermata() const {
        return _fermata;
    }
    void setFermata(std::unique_ptr<Fermata> fermata) {
        _fermata = std::move(fermata);
    }
    
    const std::vector<std::unique_ptr<Slur>>& slurs() const {
        return _slurs;
    }
    void addSlur(std::unique_ptr<Slur> slur) {
        _slurs.push_back(std::move(slur));
    }
    
    const std::vector<std::unique_ptr<Tied>>& ties() const {
        return _ties;
    }
    void addTied(std::unique_ptr<Tied> tied) {
        _ties.push_back(std::move(tied));
    }

    const std::vector<std::unique_ptr<Ornaments>>& ornaments() const {
        return _ornaments;
    }
    void addOrnaments(std::unique_ptr<Ornaments> ornaments) {
        _ornaments.push_back(std::move(ornaments));
    }
    
private:
    bool _printObject;
    std::unique_ptr<Fermata> _fermata;
    std::vector<std::unique_ptr<Articulation>> _articulations;
    std::vector<std::unique_ptr<Slur>> _slurs;
    std::vector<std::unique_ptr<Tied>> _ties;
    std::vector<std::unique_ptr<Ornaments>> _ornaments;
};

} // namespace dom
} // namespace mxml
