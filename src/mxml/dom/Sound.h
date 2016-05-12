// Copyright © 2016 Venture Media Labs.
//
// This file is part of mxml. The full mxml copyright notice, including
// terms governing use, modification, and redistribution, is contained in the
// file LICENSE at the root of the source code distribution tree.

#pragma once
#include "Node.h"
#include "Optional.h"

namespace mxml {
namespace dom {

class Sound : public Node {
public:
    /**
     Tempo is expressed in quarter notes per minute. If 0, the sound-generating program should prompt the user at the
     time of compiling a sound (MIDI) file.
     */
    Optional<float> tempo;

    /**
     Dynamics (or MIDI velocity) are expressed as a percentage of the default forte value (90 for MIDI 1.0).
     */
    Optional<float> dynamics;

    /**
     Dacapo indicates to go back to the beginning of the movement. When used it always has the value "yes".
     */
    bool dacapo;

    /**
     Segno and dalsegno are used for backwards jumps to a segno sign; coda and tocoda are used for forward jumps to a
     coda sign. If there are multiple jumps, the value of these parameters can be used to name and distinguish them.

     By default, a dalsegno or dacapo attribute indicates that the jump should occur the first time through, while a
     tocoda attribute indicates the jump should occur the second time through. The time that jumps occur can be changed
     by using the time-only attribute.
     */
    Optional<std::string> segno;
    Optional<std::string> dalsegno;
    Optional<std::string> coda;
    Optional<std::string> tocoda;

    /**
     If segno or coda is used, the divisions attribute can also be used to indicate the number of divisions per quarter
     note. Otherwise sound and MIDI generating programs may have to recompute this.
     */
    Optional<float> divisions;

    /**
     Forward-repeat is used when a forward repeat sign is implied, and usually follows a bar line. When used it always
     has the value of "yes".
     */
    bool forwardRepeat;

    /**
     The fine attribute follows the final note or rest in a movement with a da capo or dal segno direction. If numeric,
     the value represents the actual duration of the final note or rest, which can be ambiguous in written notation and
     different among parts and voices. The value may also be "yes" to indicate no change to the final duration.
     */
    Optional<std::string> fine;

    /**
     If the sound element applies only particular times through a repeat, the time-only attribute indicates which times
     to apply the sound element.
     */
    Optional<std::string> timeOnly;

    /**
     Pizzicato in a sound element effects all following notes. Yes indicates pizzicato, no indicates arco.
     */
    bool pizzicato;

    /**
     The pan and elevation attributes are deprecated in Version 2.0. The pan and elevation elements in the
     midi-instrument element should be used instead. The meaning of the pan and elevation attributes is the same as for
     the pan and elevation elements. If both are present, the mid-instrument elements take priority.
     */
    Optional<float> pan;
    Optional<float> elevation;

    /**
     The damper-pedal, soft-pedal, and sostenuto-pedal attributes effect playback of the three common piano pedals and
     their MIDI controller equivalents. The yes value indicates the pedal is depressed; no indicates the pedal is
     released. A numeric value from 0 to 100 may also be used for half pedaling. This value is the percentage that the
     pedal is depressed. A value of 0 is equivalent to no, and a value of 100 is equivalent to yes.
     */
    Optional<float> damperPedal;
    Optional<float> softPedal;
    Optional<float> sostenutoPedal;
};

} // namespace dom
} // namespace mxml
