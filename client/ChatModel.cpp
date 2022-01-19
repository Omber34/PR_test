//
// Created by Herman on 1/18/2022.
//

#include "ChatModel.h"

ChatModel::ChatModel(QObject *parent)
        : QAbstractListModel(parent)
{}

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

    emit eventAdded();
}

const QString &ChatModel::getUser() const {
    return user;
}

void ChatModel::setUser(const QString &user) {
    ChatModel::user = user;
}
