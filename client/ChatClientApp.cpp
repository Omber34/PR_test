//
// Created by Herman on 5/22/2022.
//

#include "ChatClientApp.h"

namespace client
{
  ChatClientApp::ChatClientApp()
  : client(std::make_shared<ChatClient>())
  , model(std::make_shared<ChatModel>(client))
  {

  }

  QAbstractListModel * ChatClientApp::getModel() const
  {
    return model.get();
  }

  IChatClientPtr ChatClientApp::getClient() const
  {
    return client;
  }

  const QString &ChatClientApp::getUser() const
  {
    return model->getUser();
  }

  void ChatClientApp::setUser(const QString &_user)
  {
    model->setUser(_user);
  }
}
