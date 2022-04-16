//
// Created by Herman on 4/16/2022.
//

#ifndef PR_TEST_ICHATCLIENT_H
#define PR_TEST_ICHATCLIENT_H

#include <QObject>

class IChatClient : public QObject {
public:
public slots:
    virtual void SendEvent(const ChatEvent event) = 0;

signals:
    void eventReceived(ChatEvent);

public:
    virtual ~IChatClient() {}
};

#endif //PR_TEST_ICHATCLIENT_H
