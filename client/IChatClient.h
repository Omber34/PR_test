//
// Created by Herman on 4/16/2022.
//

#ifndef PR_TEST_ICHATCLIENT_H
#define PR_TEST_ICHATCLIENT_H

#include <QObject>
#
#include "ChatEvent.h"

#include <memory>

namespace client
{
  class IChatClient : public QObject
  {
    Q_OBJECT
  public slots:
    virtual void SendEvent(core::ChatEvent event) = 0;

  signals:
    void eventReceived(core::ChatEvent);

  public:
    virtual ~IChatClient() { }
  };

  using IChatClientPtr = std::shared_ptr<IChatClient>;
}

#endif //PR_TEST_ICHATCLIENT_H
