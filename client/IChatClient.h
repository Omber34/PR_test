//
// Created by Herman on 4/16/2022.
//

#ifndef PR_TEST_ICHATCLIENT_H
#define PR_TEST_ICHATCLIENT_H

#include <QObject>
#include "ChatEvent.h"

class IChatClient : public QObject {
    Q_OBJECT
public:
public slots:
    virtual void SendEvent(ChatEvent event) = 0;

signals:
    void eventReceived(ChatEvent);

public:
    virtual ~IChatClient() {}
};

#endif //PR_TEST_ICHATCLIENT_H
