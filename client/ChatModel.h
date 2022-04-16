//
// Created by Herman on 1/18/2022.
//

#ifndef PR_TEST_CHATMODEL_H
#define PR_TEST_CHATMODEL_H

#include <QAbstractListModel>
#include <QtCore/QDateTime>
#include <utility>
#include "ChatEvent.h"
#include "IChatClient.h"

class ChatModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum
    {
        eEventRole = Qt::UserRole + 1,
    };
    Q_PROPERTY(QString user READ getUser WRITE setUser)

    explicit ChatModel(IChatClient &client, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    const QString &getUser() const;

    void setUser(const QString &_user);

    Q_INVOKABLE void sendMessage(const QString &msg);
    Q_INVOKABLE void sendFile(const QString& msg);
    Q_INVOKABLE void openFile(const QString& msg);

public slots:
    void addEvent(ChatEvent ev);

signals:
    void eventAdded();

public:
    virtual ~ChatModel();
    void Greetings();

private:
    IChatClient& client;
    QList<ChatEvent> m_events;
    QString user;
};

#endif //PR_TEST_CHATMODEL_H
