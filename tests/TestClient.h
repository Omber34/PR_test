//
// Created by Herman on 4/16/2022.
//

#ifndef PR_TEST_TESTCLIENT_H
#define PR_TEST_TESTCLIENT_H

#include "../client/IChatClient.h"

class TestClient : public client::IChatClient {
    Q_OBJECT
public:
    void SendEvent(core::ChatEvent event) override {
        eventReceived(event);
        events.emplace_back(std::move(event));
    }

    std::vector<core::ChatEvent> events;
};


#endif //PR_TEST_TESTCLIENT_H
