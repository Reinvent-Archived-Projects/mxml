//  Created by Alejandro Isaza on 2/17/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "Accidental.h"
#include "Beam.h"
#include "EmptyPlacement.h"
#include "Lyric.h"
#include "Node.h"
#include "Notations.h"
#include "Optional.h"
#include "Pitch.h"
#include "Rest.h"
#include "Tie.h"
#include "TimedNode.h"
#include "Types.h"
#include "Unpitched.h"

#include <memory>
#include <vector>

namespace mxml {
namespace dom {

class Attributes;
class Measure;

class Note : public TimedNode {
public:
    enum Type {
        TYPE_1024TH,
        TYPE_512TH,
        TYPE_256TH,
        TYPE_128TH,
        TYPE_64TH,
        TYPE_32ND,
        TYPE_16TH,
        TYPE_EIGHTH,
        TYPE_QUARTER,
        TYPE_HALF,
        TYPE_WHOLE,
        TYPE_BREVE,
        TYPE_LONG,
        TYPE_MAXIMA
    };
    
public:
    Note()
    : _defaultX(),
      _defaultY(),
      _type(TYPE_QUARTER),
      _chord(false),
      _grace(false),
      _printObject(true),
      _stem(STEM_UP),
      _staff(1)
    {}
    
    const Measure* measure() const {
        return _measure;
    }
    void setMeasure(const Measure* measure) {
        _measure = measure;
    }

    const Attributes* attributes() const {
        return _attributes;
    }
    void setAttributes(const Attributes* attributes) {
        _attributes = attributes;
    }

    const Optional<float>& defaultX() const {
        return _defaultX;
    }
    void setDefaultX(const Optional<float>& x) {
        _defaultX = x;
    }
    
    const Optional<float>& defaultY() const {
        return _defaultY;
    }
    void setDefaultY(const Optional<float>& y) {
        _defaultY = y;
    }
    
    Type type() const {
        return _type;
    }
    void setType(Type type) {
        _type = type;
    }
    
    bool chord() const {
        return _chord;
    }
    void setChord(bool chord) {
        _chord = chord;
    }
    
    bool grace() const {
        return _grace;
    }
    void setGrace(bool grace) {
        _grace = grace;
    }
    
    bool printObject() const {
        return _printObject;
    }
    void setPrintObject(bool print) {
        _printObject = print;
    }
    
    Stem stem() const {
        return _stem;
    }
    void setStem(Stem stem) {
        _stem = stem;
    }
    
    int staff() const {
        return _staff;
    }
    void setStaff(int staff) {
        _staff = staff;
    }

    Optional<float> dynamics() const {
        return  _dynamics;
    }
    void setDynamics(Optional<float> dynamics) {
        _dynamics = dynamics;
    }

    Optional<float> endDynamics() const {
        return _endDynamics;
    }
    void setEndDynamics(Optional<float> endDynamics) {
        _endDynamics = endDynamics;
    }

    time_t attack() const {
        return _attack;
    }
    void setAttack(time_t attack) {
        _attack = attack;
    }

    time_t release() const {
        return _release;
    }
    void setRelease(time_t release) {
        _release = release;
    }

    const Optional<Pitch>& pitch() const {
        return _pitch;
    }
    void setPitch(const Optional<Pitch>& pitch) {
        _pitch = pitch;
    }
    
    const Optional<Rest>& rest() const {
        return _rest;
    }
    void setRest(const Optional<Rest>& rest) {
        _rest = rest;
    }
    
    const Optional<Unpitched>& unpitched() const {
        return _unpitched;
    }
    void setUnpitched(const Optional<Unpitched>& unpitched) {
        _unpitched = unpitched;
    }
    
    const Optional<Accidental>& accidental() const {
        return _accidental;
    }
    void setAccidental(const Optional<Accidental>& accidental) {
        _accidental = accidental;
    }
    
    const Optional<EmptyPlacement>& dot() const {
        return _dot;
    }
    void setDot(const Optional<EmptyPlacement>& dot) {
        _dot = dot;
    }
    
    const Optional<Tie>& tie() const {
        return _tie;
    }
    void setTie(const Optional<Tie>& tie) {
        _tie = tie;
    }
    
    const Optional<Notations>& notations() const {
        return _notations;
    }
    void setNotations(Optional<Notations>&& notations) {
        _notations = std::move(notations);
    }
    
    const std::vector<Beam>& beams() const {
        return _beams;
    }
    void addBeam(const Beam& beam) {
        _beams.push_back(beam);
    }

    const std::vector<Lyric>& lyrics() const {
        return _lyrics;
    }
    void addLyric(const Lyric& lyric) {
        _lyrics.push_back(lyric);
    }

    unsigned int midiNumber() const;
    
private:
    const Measure* _measure;
    const Attributes* _attributes;

    Optional<float> _defaultX;
    Optional<float> _defaultY;
    
    Type _type;
    bool _chord;
    bool _grace;
    bool _printObject;
    Stem _stem;
    int _staff;

    Optional<float> _dynamics;
    Optional<float> _endDynamics;
    time_t _attack;
    time_t _release;

    Optional<Pitch> _pitch;
    Optional<Rest> _rest;
    Optional<Unpitched> _unpitched;
    Optional<Accidental> _accidental;
    Optional<EmptyPlacement> _dot;
    Optional<Tie> _tie;
    Optional<Notations> _notations;
    
    std::vector<Beam> _beams;
    std::vector<Lyric> _lyrics;
};

} // namespace dom
} // namespace mxml
