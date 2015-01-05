//  Created by Steve Hosking on 2014-12-22.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "Metrics.h"

namespace mxml {

using namespace dom;
    
const coord_t Metrics::kStaffLineSpacing = 10;
const std::size_t Metrics::kStaffLineCount = 5;

coord_t Metrics::noteY(const AttributesManager& attributesManager, const dom::Note& note) {
    const auto& part = static_cast<const dom::Part&>(*note.measure()->parent());
    return staffOrigin(part, note.staff()) + staffY(attributesManager, note);
}

coord_t Metrics::staffHeight() {
    return (kStaffLineCount - 1) * kStaffLineSpacing;
}

coord_t Metrics::stavesHeight(const dom::Part& part) {
    coord_t staffDistance = part.staffDistance();
    return (staffHeight() + staffDistance) * part.staves() - staffDistance;
}

coord_t Metrics::staffOrigin(const dom::Part& part, int staffNumber) {
    return (staffNumber - 1) * (staffHeight() + part.staffDistance());
}

coord_t Metrics::staffY(const AttributesManager& attributesManager, const Note& note) {
    coord_t y = 20;
    if (note.pitch()) {
        const auto& clef = attributesManager.clef(note);
        
        if (clef == nullptr) {
            attributesManager.clef(note);
        }
        
        y = staffY(*clef, *note.pitch());
    } else if (note.rest()) {
        const auto& clef = attributesManager.clef(note);
        y = staffY(*clef, *note.rest());
    } else if (note.defaultY().isPresent()) {
        y = note.defaultY();
    }
    return y * kStaffLineSpacing / 10;
}

coord_t Metrics::staffY(const Clef& clef, const Pitch& pitch) {
    switch (clef.sign().value()) {
        case Clef::SIGN_G: return staffYInGClef(pitch.step(), pitch.octave());
        case Clef::SIGN_F: return staffYInFClef(pitch.step(), pitch.octave());
        case Clef::SIGN_C: return staffYInCClef(pitch.step(), pitch.octave());
        default: return 20;
    }
}

coord_t Metrics::staffY(const Clef& clef, const Rest& rest) {
    if (!rest.displayStep().isPresent() || !rest.displayOctave().isPresent())
        return 20;
    
    switch (clef.sign().value()) {
        case Clef::SIGN_G: return staffYInGClef(rest.displayStep(), rest.displayOctave());
        case Clef::SIGN_F: return staffYInFClef(rest.displayStep(), rest.displayOctave());
        case Clef::SIGN_C: return staffYInCClef(rest.displayStep(), rest.displayOctave());
        default: return 20;
    }
}

coord_t Metrics::staffYInGClef(Pitch::Step step, int octave) {
    coord_t y;
    switch (step) {
        case Pitch::STEP_C: y = 50; break;
        case Pitch::STEP_D: y = 45; break;
        case Pitch::STEP_E: y = 40; break;
        case Pitch::STEP_F: y = 35; break;
        case Pitch::STEP_G: y = 30; break;
        case Pitch::STEP_A: y = 25; break;
        case Pitch::STEP_B: y = 20; break;
        default: y = 20;
    }
    return y - (octave - 4) * 35;
}

coord_t Metrics::staffYInCClef(Pitch::Step step, int octave) {
    coord_t y;
    switch (step) {
        case Pitch::STEP_C: y = 20; break;
        case Pitch::STEP_D: y = 15; break;
        case Pitch::STEP_E: y = 10; break;
        case Pitch::STEP_F: y = 5; break;
        case Pitch::STEP_G: y = 0; break;
        case Pitch::STEP_A: y = -5; break;
        case Pitch::STEP_B: y = -10; break;
        default: y = 20;
    }
    return y - (octave - 5) * 35;
}

coord_t Metrics::staffYInFClef(Pitch::Step step, int octave) {
    coord_t y;
    switch (step) {
        case Pitch::STEP_C: y = 25; break;
        case Pitch::STEP_D: y = 20; break;
        case Pitch::STEP_E: y = 15; break;
        case Pitch::STEP_F: y = 10; break;
        case Pitch::STEP_G: y = 5; break;
        case Pitch::STEP_A: y = 0; break;
        case Pitch::STEP_B: y = -5; break;
        default: y = 20;
    }
    return y - (octave - 3) * 35;
}

}
