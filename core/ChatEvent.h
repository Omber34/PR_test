//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATEVENT_H
#define PR_TEST_CHATEVENT_H
#include <string>

enum class EventType {
    PARTICIPANT_MESSAGE,
    PARTICIPANT_FILE,
    PARTICIPANT_JOIN,
    PARTICIPANT_LEAVE
};

class ChatEvent {
    EventType type;
};


#endif //PR_TEST_CHATEVENT_H
