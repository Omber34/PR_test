//
// Created by Herman on 1/18/2022.
//

#include "ChatModel.h"
#include <QDebug>
#include <QDesktopServices>
#include <QFileInfo>
#include "ChatClient.h"

ChatModel::ChatModel(QObject *parent)
    : QAbstractListModel(parent)
{
    QObject::connect(&ChatClient::getInstance(), &ChatClient::eventReceived, this, &ChatModel::addEvent);

    eventSender = [] (ChatEvent packet) {
        ChatClient::getInstance().SendEvent(std::move(packet));
    };

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
    Greetings();
}

void ChatModel::sendMessage(const QString &msg) {
    ChatEvent newEvent;
    newEvent.type = ChatEvent::EventType::PARTICIPANT_MESSAGE;
    newEvent.message = {msg, true};
    newEvent.user = ChatModel::user;

    eventSender(newEvent);
}

void ChatModel::Greetings() {
    ChatEvent chatEvent;
    chatEvent.user = user;
    chatEvent.type = ChatEvent::EventType::PARTICIPANT_JOIN;

    eventSender(chatEvent);
}

ChatModel::~ChatModel() {
    ChatEvent chatEvent;
    chatEvent.user = user;
    chatEvent.type = ChatEvent::EventType::PARTICIPANT_LEAVE;

    eventSender(chatEvent);
}

void ChatModel::sendFile(const QString &msg) {
    ChatEvent chatEvent;
    chatEvent.user = user;
    chatEvent.type = ChatEvent::EventType::PARTICIPANT_SHARE_FILE;
    chatEvent.message = {msg.mid(8), true};

    eventSender(chatEvent);
}

void ChatModel::openFile(const QString &msg) {
    QFileInfo info(msg);
    QDesktopServices::openUrl(QUrl("file:///" + info.absoluteFilePath(), QUrl::TolerantMode));
}
