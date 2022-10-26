//
// Created by Herman on 5/22/2022.
//

#pragma once

#include "ChatClient.h"
#include "ChatModel.h"

namespace client
{
  class ChatClientApp : public QObject
  {
    Q_OBJECT
  public:
    Q_PROPERTY(QString user READ getUser WRITE setUser)
    Q_PROPERTY(IChatClientPtr client READ getClient)
    Q_PROPERTY(QAbstractListModel model READ getModel)

    ChatClientApp();
    
    ChatClientApp(const ChatClientApp& other) = delete;
    ChatClientApp& operator=(const ChatClientApp& other) = delete;
    ChatClientApp(ChatClientApp&& other) = delete;
    ChatClientApp& operator=(ChatClientApp&& other) = delete;

    QAbstractListModel * getModel() const;
    IChatClientPtr getClient() const;

    const QString &getUser() const;

    void setUser(const QString &_user);

  private:
    IChatClientPtr client;
    ChatModelPtr model;
  };
}



