//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "ScoreProperties.h"
#include "LoopFactory.h"
#include "JumpFactory.h"

#include <mxml/dom/Chord.h>
#include <mxml/dom/OctaveShift.h>


namespace mxml {

const dom::Time ScoreProperties::_defaultTime;

ScoreProperties::ScoreProperties(const dom::Score& score)
: _attributes(),
  _pitches(),
  _sounds(),
  _loops(),
  _jumps(),
  _staves(0),
  _measureCount(0)
{
    _staves.resize(score.parts().size(), 0);
    _systemBeginsSet.insert(0);

    std::size_t measureCount;
    for (auto& part : score.parts()) {
        auto partIndex = part->index();
        measureCount = 0;
        for (auto& measure : part->measures()) {
            process(partIndex, *measure);
            measureCount += 1;
        }

        if (measureCount > _measureCount)
            _measureCount = measureCount;
    }

    LoopFactory loopFactory(score);
    _loops = loopFactory.build();

    JumpFactory jumpFactory(score);
    _jumps = jumpFactory.build();

    _systemBegins.insert(_systemBegins.end(), _systemBeginsSet.begin(), _systemBeginsSet.end());
}

void ScoreProperties::process(std::size_t partIndex, const dom::Measure& measure) {
    auto measureIndex = measure.index();
    for (auto& node : measure.nodes()) {
        if (auto attributes = dynamic_cast<const dom::Attributes*>(node.get()))
            process(partIndex, measureIndex, *attributes);
        else if (auto chord = dynamic_cast<const dom::Chord*>(node.get()))
            process(partIndex, measureIndex, *chord);
        else if (auto direction = dynamic_cast<const dom::Direction*>(node.get()))
            process(partIndex, measureIndex, *direction);
        else if (auto print = dynamic_cast<const dom::Print*>(node.get()))
            process(partIndex, measureIndex, *print);
    }
}

void ScoreProperties::process(std::size_t partIndex, std::size_t measureIndex, const dom::Attributes& attributes) {
    AttributesRef ref;
    ref.partIndex = partIndex;
    ref.measureIndex = measureIndex;
    ref.time = attributes.start();
    ref.attributes = &attributes;
    _attributes.insert(ref);

    if (attributes.staves().isPresent()) {
        assert(_staves[partIndex] == 0); // We don't support changing the number of staves mid-song
        _staves[partIndex] = attributes.staves();
    } else if (_staves[partIndex] == 0) {
        _staves[partIndex] = 1;
    }
}

void ScoreProperties::process(std::size_t partIndex, std::size_t measureIndex, const dom::Direction& direction) {
    DirectionRef ref;
    ref.partIndex = partIndex;
    ref.measureIndex = measureIndex;
    ref.staff = direction.staff();
    ref.time = direction.start();
    ref.direction = &direction;
    _directions.insert(ref);

    if (direction.sound()) {
        SoundRef ref;
        ref.partIndex = partIndex;
        ref.measureIndex = measureIndex;
        ref.staff = direction.staff();
        ref.time = direction.start();
        ref.sound = direction.sound().get();
        _sounds.insert(ref);
    }
}

void ScoreProperties::process(std::size_t partIndex, std::size_t measureIndex, const dom::Print& print) {
    if (print.newSystem || print.newPage)
        _systemBeginsSet.insert(measureIndex);
}

void ScoreProperties::process(std::size_t partIndex, std::size_t measureIndex, const dom::Chord& chord) {
    for (auto& note : chord.notes()) {
        process(partIndex, measureIndex, *note);
    }
}

void ScoreProperties::process(std::size_t partIndex, std::size_t measureIndex, const dom::Note& note) {
    if (!note.pitch())
        return;

    PitchRef ref;
    ref.partIndex = partIndex;
    ref.measureIndex = measureIndex;
    ref.staff = note.staff();
    ref.pitch = note.pitch().get();
    ref.time = note.start();
    _pitches.insert(ref);
}

const dom::Key* ScoreProperties::key(std::size_t partIndex, std::size_t measureIndex, int staff, time_t time) const {
    return getAttribute<const dom::Key*>(partIndex, measureIndex, time, nullptr, [&](const dom::Attributes& attribute, const dom::Key* previous) {
        if (attribute.key(staff))
            return attribute.key(staff);
        else if (attribute.key(1))
            return attribute.key(1);
        return previous;
    });
}

const dom::Clef* ScoreProperties::clef(std::size_t partIndex, std::size_t measureIndex, int staff, time_t time) const {
    return getAttribute<const dom::Clef*>(partIndex, measureIndex, time, nullptr, [&](const dom::Attributes& attribute, const dom::Clef* previous) {
        if (attribute.clef(staff))
            return attribute.clef(staff);
        else if (attribute.clef(1))
            return attribute.clef(1);
        return previous;
    });
}

const dom::Time* ScoreProperties::time(std::size_t measureIndex) const {
    return getAttribute<const dom::Time*>(measureIndex, &_defaultTime, [&](const dom::Attributes& attribute, const dom::Time* previous) {
        if (attribute.time())
            return attribute.time();
        return previous;
    });
}

dom::time_t ScoreProperties::divisions(std::size_t measureIndex) const {
    return getAttribute<int>(measureIndex, 0, 1, [&](const dom::Attributes& attribute, int previous) {
        if (attribute.divisions().isPresent())
            return attribute.divisions().value();
        return previous;
    });
}

dom::time_t ScoreProperties::divisionsPerBeat(std::size_t measureIndex) const {
    const auto divs = divisions(measureIndex);
    const auto t = time(measureIndex);
    return divs * 4 / t->beatType();
}

dom::time_t ScoreProperties::divisionsPerMeasure(std::size_t measureIndex) const {
    const auto divs = divisions(measureIndex);
    const auto t = time(measureIndex);
    return (divs * 4 / t->beatType()) * t->beats();
}

const dom::Clef* ScoreProperties::clef(const dom::Note& note) const {
    const auto partIndex = note.measure()->part()->index();
    const auto measureIndex = note.measure()->index();
    return clef(partIndex, measureIndex, note.staff(), note.start());
}

int ScoreProperties::staves(std::size_t partIndex) const {
    return _staves[partIndex];
}

int ScoreProperties::alter(const dom::Note& note) const {
    const auto partIndex = note.measure()->part()->index();
    const auto measureIndex = note.measure()->index();
    return alter(partIndex, measureIndex, note.staff(), note.start(), note.pitch()->octave(), note.pitch()->step());
}

int ScoreProperties::previousAlter(const dom::Note& note) const {
    const auto partIndex = note.measure()->part()->index();
    const auto measureIndex = note.measure()->index();
    return alter(partIndex, measureIndex, note.staff(), note.start() - 1, note.pitch()->octave(), note.pitch()->step());
}

int ScoreProperties::alter(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time, int octave, dom::Pitch::Step step) const {
    auto currentKey = key(partIndex, measureIndex, staff, time);
    if (!currentKey)
        return 0;
    
    const int base = currentKey->alter(step);
    int current = base;

    for (auto& ref : _pitches) {
        if (ref.measureIndex > measureIndex || (ref.measureIndex == measureIndex && ref.time > time))
            return current;
        if (ref.measureIndex == measureIndex && ref.staff == staff && ref.pitch->octave() == octave && ref.pitch->step() == step && ref.pitch->alter().isPresent())
            current = ref.pitch->alter();
    }

    return current;
}

float ScoreProperties::tempo(std::size_t measureIndex, dom::time_t time) const {
    float current = 60.0;
    for (auto& ref : _sounds) {
        if (ref.measureIndex > measureIndex || (ref.measureIndex == measureIndex && ref.time > time))
            return current;
        if (ref.sound->tempo.isPresent())
            current = ref.sound->tempo.value();
    }
    return current;
}

float ScoreProperties::dynamics(const dom::Note& note) const {
    if (note.dynamics().isPresent() && note.dynamics().value() > 0)
        return note.dynamics();

    auto measure = note.measure();
    auto part = static_cast<const mxml::dom::Part*>(measure->parent());
    return dynamics(part->index(), measure->index(), note.staff(), note.start());
}

float ScoreProperties::dynamics(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const {
    // Current value loosely based of a MIDI value of 80 (80/127 ~= 0.65)
    float current = 65.0;
    for (auto& ref : _sounds) {
        if (ref.measureIndex > measureIndex || (ref.measureIndex == measureIndex && ref.time > time))
            return current;

        if (ref.staff.isPresent() && ref.staff != staff)
            continue;
        if (ref.partIndex == partIndex && ref.sound->dynamics.isPresent())
            current = ref.sound->dynamics.value();
    }
    return current;
}

const Loop* ScoreProperties::loop(std::size_t measureIndex) const {
    auto it = std::find_if(_loops.begin(), _loops.end(), [&](const Loop& loop) {
        return loop.begin() <= measureIndex && loop.end() > measureIndex;
    });
    if (it == _loops.end())
        return nullptr;
    return &*it;
}

std::vector<Jump> ScoreProperties::jumps(std::size_t measureIndex) const {
    std::vector<Jump> jumps;
    for (auto& jump : _jumps) {
        if (jump.from + 1 == measureIndex)
            jumps.push_back(jump);
    }
    return jumps;
}

int ScoreProperties::octaveShift(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time) const {
    int current = 0;
    for (auto& ref : _directions) {
        if (ref.measureIndex > measureIndex || (ref.measureIndex == measureIndex && ref.time > time))
            return current;

        if (ref.staff.isPresent() && ref.staff != staff)
            continue;
        if (ref.partIndex != partIndex)
            continue;

        auto octaveShift = dynamic_cast<const dom::OctaveShift*>(ref.direction->type());
        if (!octaveShift)
            continue;

        if (octaveShift->type == dom::OctaveShift::kStop || octaveShift->size == 0)
            current = 0;
        else
            current = (octaveShift->type == dom::OctaveShift::kDown ? -1: 1) * (octaveShift->size - 1);
    }
    return current;
}

std::size_t ScoreProperties::systemIndex(std::size_t measureIndex) const {
    auto it = std::upper_bound(_systemBegins.begin(), _systemBegins.end(), measureIndex);
    if (it == _systemBegins.end())
        return _systemBegins.size() - 1;
    return std::distance(_systemBegins.begin(), it) - 1;
}

std::pair<std::size_t, std::size_t> ScoreProperties::measureRange(std::size_t index) const {
    std::pair<std::size_t, std::size_t> range;
    range.first = _systemBegins[index];
    if (index == _systemBegins.size() - 1)
        range.second = measureCount();
    else
        range.second = _systemBegins[index + 1];
    return range;
}

}
