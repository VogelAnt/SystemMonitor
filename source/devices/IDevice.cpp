#include "IDevice.h"

QString StateTransitionToString(PackMLStateTransition transition) {
    switch (transition) {
    case PackMLStateTransition::Abort:
        return "abort";
    case PackMLStateTransition::Clear:
        return "clear";
    case PackMLStateTransition::Reset:
        return "reset";
    case PackMLStateTransition::Start:
        return "start";
    case PackMLStateTransition::Stop:
        return "stop";
    }
}
