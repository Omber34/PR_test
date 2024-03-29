//
// Created by Herman on 1/17/2022.
//

#ifndef PR_TEST_CHATEVENT_H
#define PR_TEST_CHATEVENT_H

#include <QObject>
#include <QVariant>
#include <QDateTime>

namespace core
{
  struct ChatMessage
  {
  Q_GADGET
    Q_PROPERTY(QString message MEMBER message)
    Q_PROPERTY(bool isFromMe MEMBER isFromMe)
  public:
    ChatMessage() = default;

    ChatMessage(QString message, bool isFromMe)
            : message(std::move(message)), isFromMe(isFromMe)
    {}

    QString message;
    bool isFromMe = false;
  };

  struct ChatEvent
  {
  Q_GADGET

    Q_PROPERTY(EventType type MEMBER type)
    Q_PROPERTY(QVariant message READ getMessage WRITE setMessage)
    Q_PROPERTY(QString user MEMBER user)
    Q_PROPERTY(QString timeString READ getTimeString)

  public:
    enum EventType
    {
      EMPTY = -1,
      PARTICIPANT_MESSAGE,
      PARTICIPANT_FILE,
      PARTICIPANT_JOIN,
      PARTICIPANT_LEAVE,
      PARTICIPANT_SHARE_FILE
    };

    Q_ENUM(EventType)

    QVariant getMessage() const
    {
      return QVariant::fromValue(message);
    }

    void setMessage(const QVariant &chatMessage)
    {
      ChatEvent::message = chatMessage.value<ChatMessage>();
    }

    QString getTimeString() const
    {
      return occurredTime.toString("hh:mm ap");
    }

    EventType type = EventType::EMPTY;
    QString user;
    ChatMessage message;
    QDateTime occurredTime = QDateTime::currentDateTime();
    size_t packetCount = 0;
  };
}

#endif //PR_TEST_CHATEVENT_H
