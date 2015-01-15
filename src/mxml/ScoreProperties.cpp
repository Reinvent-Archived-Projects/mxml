//  Created by Steve Hosking on 2014-12-18.
//  Copyright (c) 2014 Venture Media Labs. All rights reserved.

#include "ScoreProperties.h"
#include "LoopFactory.h"
#include "JumpFactory.h"

#include <mxml/dom/Chord.h>


namespace mxml {

const dom::Key ScoreProperties::_defaultKey;
const dom::Clef ScoreProperties::_defaultClef;
const dom::Time ScoreProperties::_defaultTime;

ScoreProperties::ScoreProperties(const dom::Score& score) : _attributes(), _pitches(), _staves(0) {
    _staves.resize(score.parts().size(), 0);
    for (auto& part : score.parts()) {
        auto partIndex = part->index();
        for (auto& measure : part->measures()) {
            process(partIndex, *measure);
        }
    }

    LoopFactory loopFactory(score);
    _loops = loopFactory.build();

    JumpFactory jumpFactory(score);
    _jumps = jumpFactory.build();
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
    if (!direction.sound())
        return;

    SoundRef ref;
    ref.partIndex = partIndex;
    ref.measureIndex = measureIndex;
    ref.staff = direction.staff();
    ref.time = direction.start();
    ref.sound = direction.sound().get();
    _sounds.insert(ref);
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
    return getAttribute<const dom::Key*>(partIndex, measureIndex, time, &_defaultKey, [&](const dom::Attributes& attribute, const dom::Key* previous) {
        if (attribute.key(staff))
            return attribute.key(staff);
        else if (attribute.key(1))
            return attribute.key(1);
        return previous;
    });
}

const dom::Clef* ScoreProperties::clef(std::size_t partIndex, std::size_t measureIndex, int staff, time_t time) const {
    return getAttribute<const dom::Clef*>(partIndex, measureIndex, time, &_defaultClef, [&](const dom::Attributes& attribute, const dom::Clef* previous) {
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

int ScoreProperties::divisions(std::size_t measureIndex) const {
    return getAttribute<int>(measureIndex, 0, 1, [&](const dom::Attributes& attribute, int previous) {
        if (attribute.divisions().isPresent())
            return attribute.divisions().value();
        return previous;
    });
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
    return alter(partIndex, measureIndex, note.start(), note.staff(), note.pitch()->octave(), note.pitch()->step());
}

int ScoreProperties::alter(std::size_t partIndex, std::size_t measureIndex, int staff, dom::time_t time, int octave, dom::Pitch::Step step) const {
    auto currentKey = key(partIndex, measureIndex, staff, time);
    int current = currentKey->alter(step);

    for (auto& ref : _pitches) {
        if (ref.measureIndex > measureIndex || (ref.measureIndex == measureIndex && ref.time > time))
            return current;
        if (ref.measureIndex == measureIndex && ref.staff == staff && ref.pitch->octave() == octave && ref.pitch->step() == step)
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
    float current = 100.0;
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

}
