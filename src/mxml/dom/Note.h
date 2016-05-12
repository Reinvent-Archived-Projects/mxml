// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Accidental.h"
#include "Articulation.h"
#include "Beam.h"
#include "EmptyPlacement.h"
#include "Fermata.h"
#include "Lyric.h"
#include "Node.h"
#include "Notations.h"
#include "Optional.h"
#include "Pitch.h"
#include "Position.h"
#include "Rest.h"
#include "Slur.h"
#include "Tie.h"
#include "Tied.h"
#include "TimeModification.h"
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
    enum class Type {
        _1024th,
        _512th,
        _256th,
        _128th,
        _64th,
        _32nd,
        _16th,
        Eighth,
        Quarter,
        Half,
        Whole,
        Breve,
        Long,
        Maxima
    };
    
public:
    Note()
    : printObject(true),
      _type(absentOptional(Type::Quarter)),
      _chord(false),
      _grace(false),
      _stem(Stem::Up, false),
      _staff(1)
    {}
    
    const Measure* measure() const {
        return _measure;
    }
    void setMeasure(const Measure* measure) {
        _measure = measure;
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
    
    Optional<Stem> stem() const {
        return _stem;
    }
    void setStem(Optional<Stem> stem) {
        _stem = stem;
    }
    
    int staff() const {
        return _staff;
    }
    void setStaff(int staff) {
        _staff = staff;
    }

    const std::string& voice() const {
        return _voice;
    }
    void setVoice(const std::string& voice) {
        _voice = voice;
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

    /**
     Return the alter value for this note, coming either from an accidental or from the pitch alter value.
     */
    int alter() const {
        if (accidental)
            return accidental->type.alter;
        else if (pitch)
            return pitch->alter();
        return 0;
    }
    
    unsigned int midiNumber() const;

public:
    Position position;
    bool printObject;

    std::unique_ptr<Pitch> pitch;
    std::unique_ptr<Rest> rest;
    std::unique_ptr<Unpitched> unpitched;
    std::unique_ptr<Accidental> accidental;
    std::unique_ptr<EmptyPlacement> dot;
    std::unique_ptr<Tie> tie;
    std::unique_ptr<Notations> notations;
    std::unique_ptr<TimeModification> timeModification;

private:
    const Measure* _measure;
    
    bool _chord;
    bool _grace;
    Optional<Stem> _stem;
    int _staff;
    std::string _voice;

    Optional<Type> _type;
    Optional<float> _dynamics;
    Optional<float> _endDynamics;
    time_t _attack;
    time_t _release;
    
    std::vector<std::unique_ptr<Beam>> _beams;
    std::vector<std::unique_ptr<Lyric>> _lyrics;
};

} // namespace dom
} // namespace mxml
