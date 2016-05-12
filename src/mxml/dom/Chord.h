// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Note.h"
#include "TimedNode.h"

#include <memory>
#include <vector>

namespace mxml {
namespace dom {

class Chord : public TimedNode {
public:
    Chord() {}
    
    bool empty() const {
        return _notes.empty();
    }
    
    const Note* firstNote() const {
        if (_notes.empty())
            return 0;
        return _notes.front().get();
    }
    
    Note::Type type() const {
        return firstNote()->type();
    }
    
    Optional<Stem> stem() const {
        return firstNote()->stem();
    }
    
    const std::vector<std::unique_ptr<Beam>>& beams() const {
        return firstNote()->beams();
    }
    
    bool isBeamStart() const {
        if (_notes.empty() || firstNote()->beams().empty())
            return false;
        return firstNote()->beams().front()->type() == Beam::Type::Begin;
    }
    
    bool isBeamContinue() const {
        if (_notes.empty() || firstNote()->beams().empty())
            return false;
        return firstNote()->beams().front()->type() == Beam::Type::Continue;
    }
    
    bool isBeamEnd() const {
        if (_notes.empty() || firstNote()->beams().empty())
            return false;
        return firstNote()->beams().front()->type() == Beam::Type::End;
    }
    
    bool hasBeam() const {
        return !_notes.empty() && !firstNote()->beams().empty();
    }
    
    void addNote(std::unique_ptr<Note>&& note) {
        if (_notes.empty())
            setStart(note->start());
        
        if (duration().isPresent()) {
            if (note->duration().isPresent())
                setDuration(presentOptional(std::max(duration().value(), note->duration().value())));
        } else {
            setDuration(note->duration());
        }
        _notes.push_back(std::move(note));
    }
    
    const std::vector<std::unique_ptr<Note>>& notes() const {
        return _notes;
    }
    
private:
    std::vector<std::unique_ptr<Note>> _notes;
};

} // namespace dom
} // namespace mxml
