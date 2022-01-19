//
// Created by Herman on 1/18/2022.
//

#include "PacketEventTransform.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <iostream>

ChatPacket PacketEventTransform::packetFromEvent(const ChatEvent &event) {
    QJsonObject eventData;
    eventData["type"] = static_cast<int>(event.type);
    eventData["user"] = event.user;
    eventData["message"] = event.message.message;

    auto doc = QJsonDocument();
    doc.setObject(eventData);
    char* packetData = doc.toJson().data();
    qDebug() << " packetFromEvent packet data" <<  doc.toJson() << "\n";
    ChatPacket result;
    result.body_length(std::strlen(packetData));
    std::memcpy(result.data(), packetData, result.body_length());
    result.encode_header();

    return result;
}

ChatEvent PacketEventTransform::eventFromPacket(ChatPacket &packet) {
    packet.decode_header();
    QByteArray packetData = packet.body();
    qDebug() << " eventFromPacket packet data 1" << packet.data() << "\n";
    auto doc = QJsonDocument::fromBinaryData(packetData);
    auto jsonEvent = doc.object();
    qDebug() << " eventFromPacket packet data doc" <<  doc.toJson() << "\n";

    ChatEvent result;
    result.type = static_cast<ChatEvent::EventType>(jsonEvent["type"].toInt());
    result.user = jsonEvent["user"].toString();
    result.message = {jsonEvent["message"].toString(), false};

    return result;
}
