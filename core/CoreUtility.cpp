//
// Created by Herman on 1/18/2022.
//

#include "CoreUtility.h"
#include "FileManager.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QDebug>
#include <iostream>

ChatPacket CoreUtility::packetFromEvent(const ChatEvent &event) {
    QJsonObject eventData;
    eventData["type"] = static_cast<int>(event.type);
    eventData["user"] = event.user;
    eventData["message"] = event.message.message;
    if (event.packetCount != 0) {
        eventData["packetCount"] = static_cast<int>(event.packetCount);
    }

    auto doc = QJsonDocument();
    doc.setObject(eventData);
    std::string buff = doc.toJson(QJsonDocument::Compact).toStdString();
    const char* packetData = buff.data();
    qDebug() << "packetFromEvent" << doc.toJson(QJsonDocument::Compact);
    ChatPacket result;
    result.body_length(std::strlen(packetData));
    std::memcpy(result.body(), packetData, result.body_length());
    result.encode_header();

    return result;
}

ChatEvent CoreUtility::eventFromPacket(const ChatPacket &packet) {
    QByteArray packetData = QByteArray::fromStdString({packet.body(), packet.body_length()});
    qDebug() << "eventFromPacket"<< packetData;
    auto doc = QJsonDocument::fromJson(packetData);
    auto jsonEvent = doc.object();

    ChatEvent result;
    result.type = static_cast<ChatEvent::EventType>(jsonEvent["type"].toInt(-1));
    result.user = jsonEvent["user"].toString();
    result.message = {jsonEvent["message"].toString(), false};
    result.packetCount = jsonEvent["packetCount"].toInt(0);
    return result;
}

ChatEvent CoreUtility::eventFromFilePacket(const ChatFilePacket &packet) {
    auto it = std::find_if(packet.packets.begin(), packet.packets.end(), [] (const ChatPacket& packet1) {
        return packet1.sequence_index() == 0;
    });
    if (it != packet.packets.end()){
        return eventFromPacket(*it);
    }
    return ChatEvent();
}

ChatFilePacket CoreUtility::filePacketFromEvent(ChatEvent event) {
    auto result = FileManager::loadFileToFilePacket(event.message.message.toStdString());

    result.expectedCount = result.packets.size() + 1;
    event.packetCount = result.packets.size() + 1;
    auto metainfoPacket = packetFromEvent(event);
    metainfoPacket.sequence_index(0);
    metainfoPacket.event_id(result.packets.front().event_id());
    metainfoPacket.encode_header();

    result.packets.push_front(metainfoPacket);

    return result;
}

