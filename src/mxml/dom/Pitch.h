//  Created by Alejandro Isaza on 2/17/2014.
//  Copyright (c) 2014 Venture Media Labs Inc. All rights reserved.

#pragma once
#include "InvalidDataError.h"
#include "Node.h"
#include "Optional.h"

namespace mxml {
namespace dom {

class Pitch : public Node {
public:
    enum Step {
        STEP_C = 0, STEP_D, STEP_E, STEP_F, STEP_G, STEP_A, STEP_B
    };

    /// The total number of steps (7)
    static const std::size_t kStepCount;

    /// The total number of octaves (10)
    static const std::size_t kOctaveCount;

    /// The total count of unique pitches (kStepCount*kOctaveCount = 70)
    static const std::size_t kUniqueCount;

public:
    Pitch() : _step(STEP_C), _alter(0), _octave(4) {}
    Pitch(Step step, int alter, int octave)
    : _step(step), _alter(alter), _octave(octave) {}
    
    /**
     The step in the diatonic scale.
     */
    const Step& step() const {
        return _step;
    }
    
    void setStep(Step s) {
        _step = s;
    }
    
    /**
     Chromatic alteration in number of semitones (e.g. -1 for flat, 1 for sharp).
     */
    int alter() const {
        return _alter;
    }
    
    void setAlter(int alter) {
        _alter = alter;
    }
    
    /**
     The octave number in the range [0, 9], where 4 indicates the octave started by middle C.
     */
    int octave() const {
        return _octave;
    }
    
    void setOctave(int octave) {
        if (octave < 0 || octave > 9)
            throw InvalidDataError("octave not in valid range");
        _octave = octave;
    }


    bool operator==(const Pitch& rhs) const {
        return _step == rhs._step && _octave == rhs._octave;
    }
    bool operator!=(const Pitch& rhs) const {
        return !operator==(rhs);
    }
    
    bool operator<(const Pitch& rhs) const {
        if (_octave < rhs._octave)
            return true;
        if (_octave == rhs._octave)
            return _step < rhs._step;
        return false;
    }
    bool operator>(const Pitch& rhs) const {
        return !operator<(rhs) && !operator==(rhs);
    }
    
private:
    Step _step;
    int _alter;
    int _octave;
};

} // namespace dom
} // namespace mxml
