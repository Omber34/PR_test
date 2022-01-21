//
// Created by Herman on 1/18/2022.
//

#ifndef PR_TEST_CHATMODEL_H
#define PR_TEST_CHATMODEL_H


#include <QAbstractListModel>
#include <QtCore/QDateTime>
#include <QSet>
#include <utility>

#include "ChatEvent.h"
#include "ChatPacket.h"

class ChatModel : public QAbstractListModel
{
    Q_OBJECT

    enum
    {
        eEventRole = Qt::UserRole + 1,
    };
public:
    Q_PROPERTY(QString user READ getUser WRITE setUser)

    explicit ChatModel(QObject *parent = nullptr);
    //explicit ChatModel(std::function<void(ChatPacket)> packetSender, QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    const QString &getUser() const;

    void setUser(const QString &_user);


    Q_INVOKABLE void sendMessage(const QString &msg);

public slots:
    void addEvent(ChatEvent ev);

signals:
    void eventAdded();

public:
    virtual ~ChatModel();

private:
    QList<ChatEvent> m_events;
    QString user;
    std::function<void(ChatPacket)> packetSender;
};

#endif //PR_TEST_CHATMODEL_H
