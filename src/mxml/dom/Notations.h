// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

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

class Tuplet;

/**
 Musical notations. Multiple notations are allowed in order to represent multiple editorial levels. The printObject
 attribute allows notations to represent details of performance technique, such as fingerings, without having them
 appear in the score.
 */
class Notations : public Node {
public:
    Notations();
    ~Notations();

    bool printObject;
    std::unique_ptr<Fermata> fermata;
    std::vector<std::unique_ptr<Articulation>> articulations;
    std::vector<std::unique_ptr<Slur>> slurs;
    std::vector<std::unique_ptr<Tied>> ties;
    std::vector<std::unique_ptr<Ornaments>> ornaments;
    std::vector<std::unique_ptr<Tuplet>> tuplets;
};

} // namespace dom
} // namespace mxml
