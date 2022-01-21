//
// Created by Herman on 1/18/2022.
//

#include "ChatModel.h"
#include "CoreUtility.h"
#include <QDebug>
#include <utility>
#include "ChatClient.h"

ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QObject::connect(&ChatClient::getInstance(), &ChatClient::eventReceived, this, &ChatModel::addEvent);
    ChatEvent chatEvent;
    chatEvent.user = "test";
    chatEvent.message = {"test", true};
    chatEvent.type = ChatEvent::EventType::PARTICIPANT_MESSAGE;
    packetSender = [] (ChatPacket packet) {
        ChatClient::getInstance().SendPacket(packet);
    };
    addEvent(chatEvent);
}
//
//ChatModel::ChatModel(std::function<void(ChatPacket)> packetSender, QObject *parent)
//        : QAbstractListModel(parent)
//        , packetSender(std::move(packetSender))
//{}

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

void ChatModel::addEvent(ChatEvent ev)
{
    auto row = m_events.size();
    if (ev.type == ChatEvent::EventType::PARTICIPANT_MESSAGE) {
        ev.message.isFromMe = ev.user == user;
    }
    beginInsertRows(QModelIndex(), row, row);
    m_events.append(ev);
    endInsertRows();

    eventAdded();
}

const QString &ChatModel::getUser() const {
    return user;
}

void ChatModel::setUser(const QString &_user) {
    ChatModel::user = _user;
    qDebug() << "username: " << ChatModel::user;
}

void ChatModel::sendMessage(const QString &msg) {
    ChatEvent newEvent;
    qDebug() << "sendMessage: "  << msg << ChatModel::user;
    newEvent.type = ChatEvent::EventType::PARTICIPANT_MESSAGE;
    newEvent.message = {msg, true};
    newEvent.user = ChatModel::user;

    packetSender(CoreUtility::packetFromEvent(newEvent));
}

ChatModel::~ChatModel() {}
