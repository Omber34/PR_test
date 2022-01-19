//
// Created by Herman on 1/18/2022.
//

#ifndef PR_TEST_CHATMODEL_H
#define PR_TEST_CHATMODEL_H


#include <QAbstractListModel>
#include <QtCore/QDateTime>
#include <QSet>
#include <utility>

#include "../core/ChatEvent.h"

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

    enum
    {
        eEventRole = Qt::UserRole + 1,
    };

public:
    explicit ChatModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    const QString &getUser() const;

    void setUser(const QString &user);

    void addEvent(ChatEvent ev);
signals:
    void eventAdded();

private:
    QList<ChatEvent> m_events;
    QString user;
};

#endif //PR_TEST_CHATMODEL_H
