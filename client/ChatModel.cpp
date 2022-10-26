//
// Created by Herman on 1/18/2022.
//

#include "ChatModel.h"
#include <QQmlEngine>
#include <QDesktopServices>
#include <QFileInfo>
#include "ChatClient.h"

namespace client
{
  ChatModel::ChatModel(IChatClientPtr client, QObject *parent)
          : QAbstractListModel(parent)
          , client(client)
  {
    QObject::connect(client.get(), &IChatClient::eventReceived, this, &ChatModel::addEvent);
  }

  QHash<int, QByteArray> ChatModel::roleNames() const
  {
    QHash<int, QByteArray> hash;
    hash.insert(eEventRole, "event");

    return hash;
  }

  int ChatModel::rowCount(const QModelIndex &parent) const
  {
    if (parent.isValid())
      return 0;

    return m_events.size();
  }

  QVariant ChatModel::data(const QModelIndex &index, int role) const
  {
    if (!index.isValid())
      return QVariant();

    switch (role)
    {
      case eEventRole:
        return QVariant::fromValue(m_events[index.row()]);
    }

    return QVariant();
  }

  void ChatModel::addEvent(core::ChatEvent ev)
  {
    auto row = m_events.size();
    if (ev.type == core::ChatEvent::EventType::PARTICIPANT_MESSAGE)
    {
      ev.message.isFromMe = ev.user == user;
    }
    beginInsertRows(QModelIndex(), row, row);
    m_events.append(ev);
    endInsertRows();

    eventAdded();
  }

  const QString &ChatModel::getUser() const
  {
    return user;
  }

  void ChatModel::setUser(const QString &_user)
  {
    ChatModel::user = _user;
    Greetings();
  }

  void ChatModel::sendMessage(const QString &msg)
  {
    core::ChatEvent newEvent;
    newEvent.type = core::ChatEvent::EventType::PARTICIPANT_MESSAGE;
    newEvent.message = { msg, true };
    newEvent.user = ChatModel::user;

    client->SendEvent(newEvent);
  }

  void ChatModel::Greetings()
  {
    core::ChatEvent chatEvent;
    chatEvent.user = user;
    chatEvent.type = core::ChatEvent::EventType::PARTICIPANT_JOIN;

    client->SendEvent(chatEvent);
  }

  ChatModel::~ChatModel()
  {
    core::ChatEvent chatEvent;
    chatEvent.user = user;
    chatEvent.type = core::ChatEvent::EventType::PARTICIPANT_LEAVE;

    client->SendEvent(chatEvent);
  }

  void ChatModel::sendFile(const QString &msg)
  {
    core::ChatEvent chatEvent;
    chatEvent.user = user;
    chatEvent.type = core::ChatEvent::EventType::PARTICIPANT_SHARE_FILE;
    auto filename = msg.contains("file:///") ? msg.mid(8) : msg;
    chatEvent.message = { filename, true };

    client->SendEvent(chatEvent);
  }

  void ChatModel::openFile(const QString &msg)
  {
    QFileInfo info(msg);
    QDesktopServices::openUrl(QUrl("file:///" + info.absoluteFilePath(), QUrl::TolerantMode));
  }
}
