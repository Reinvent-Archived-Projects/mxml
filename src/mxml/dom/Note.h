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
      _type(absentOptional(TYPE_QUARTER)),
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
    
    Optional<Type> type() const {
        return _type;
    }
    void setType(const Optional<Type>& type) {
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

    const std::unique_ptr<Pitch>& pitch() const {
        return _pitch;
    }
    void setPitch(std::unique_ptr<Pitch> pitch) {
        _pitch = std::move(pitch);
    }
    
    const std::unique_ptr<Rest>& rest() const {
        return _rest;
    }
    void setRest(std::unique_ptr<Rest> rest) {
        _rest = std::move(rest);
    }
    
    const std::unique_ptr<Unpitched>& unpitched() const {
        return _unpitched;
    }
    void setUnpitched(std::unique_ptr<Unpitched> unpitched) {
        _unpitched = std::move(unpitched);
    }
    
    const std::unique_ptr<Accidental>& accidental() const {
        return _accidental;
    }
    void setAccidental(std::unique_ptr<Accidental> accidental) {
        _accidental = std::move(accidental);
    }
    
    const std::unique_ptr<EmptyPlacement>& dot() const {
        return _dot;
    }
    void setDot(std::unique_ptr<EmptyPlacement> dot) {
        _dot = std::move(dot);
    }
    
    const std::unique_ptr<Tie>& tie() const {
        return _tie;
    }
    void setTie(std::unique_ptr<Tie> tie) {
        _tie = std::move(tie);
    }
    
    const std::unique_ptr<Notations>& notations() const {
        return _notations;
    }
    void setNotations(std::unique_ptr<Notations> notations) {
        _notations = std::move(notations);
    }
    
    const std::vector<std::unique_ptr<Beam>>& beams() const {
        return _beams;
    }
    void addBeam(std::unique_ptr<Beam> beam) {
        _beams.push_back(std::move(beam));
    }

    const std::vector<std::unique_ptr<Lyric>>& lyrics() const {
        return _lyrics;
    }
    void addLyric(std::unique_ptr<Lyric> lyric) {
        _lyrics.push_back(std::move(lyric));
    }

    unsigned int midiNumber() const;
    
private:
    const Measure* _measure;
    const Attributes* _attributes;

    Optional<float> _defaultX;
    Optional<float> _defaultY;
    
    bool _chord;
    bool _grace;
    bool _printObject;
    Stem _stem;
    int _staff;
    
    Optional<Type> _type;
    Optional<float> _dynamics;
    Optional<float> _endDynamics;
    time_t _attack;
    time_t _release;
    
    std::unique_ptr<Pitch> _pitch;
    std::unique_ptr<Rest> _rest;
    std::unique_ptr<Unpitched> _unpitched;
    std::unique_ptr<Accidental> _accidental;
    std::unique_ptr<EmptyPlacement> _dot;
    std::unique_ptr<Tie> _tie;
    std::unique_ptr<Notations> _notations;
    
    std::vector<std::unique_ptr<Beam>> _beams;
    std::vector<std::unique_ptr<Lyric>> _lyrics;
};

} // namespace dom
} // namespace mxml
