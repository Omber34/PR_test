//
// Created by Herman on 1/23/2022.
//

#ifndef PR_TEST_CHATFILEPACKET_H
#define PR_TEST_CHATFILEPACKET_H


#include <set>
#include "ChatPacket.h"

namespace std {
    template <>
    struct less<ChatPacket> {
        bool operator()( const ChatPacket& lhs, const ChatPacket& rhs ) const {
            return lhs.sequence_index() < rhs.sequence_index();
        }
    };
}

struct ChatFilePacket {
    std::set<ChatPacket> packets;
    size_t expectedCount;
};


#endif //PR_TEST_CHATFILEPACKET_H
