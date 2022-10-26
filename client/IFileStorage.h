//
// Created by Herman on 4/17/2022.
//

#ifndef PR_TEST_IFILESTORAGE_H
#define PR_TEST_IFILESTORAGE_H


#include <ChatFilePacket.h>

class IFileStorage {
public:
    virtual ChatFilePacket getFile(const ChatEvent& fileEvent) = 0;
    virtual bool hasFile(const ChatEvent& fileEvent) const noexcept = 0;
    virtual bool addFile(const ChatFilePacket& filePacket) = 0;
    virtual std::vector<ChatPacket> fileListForUser(const QString& user) const = 0;

};


#endif //PR_TEST_IFILESTORAGE_H
