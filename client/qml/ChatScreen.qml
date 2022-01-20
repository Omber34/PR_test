import QtQuick 2.9
import QtQuick.Templates 2.2 as T
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import models 1.0
import enums 1.0
import ".."

Item
{
    id: screen

    Item
    {
        id: topPanelRow

        width: 350
        height: 50
        z: 3

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top

        Text
        {
            id: handlerName

            height: parent.height

            anchors.centerIn: parent

            text: tableName

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: "white"
        }
    }

    ChatModel
    {
        id: chatModel
    }

    ListView
    {
        id: messageListView

        anchors.top: parent.top
        anchors.topMargin: 75
        anchors.bottom: msgInput.top
        anchors.horizontalCenter: parent.horizontalCenter

        width: 500
        spacing: 15

        model: tableModel.chat

        Connections
        {
            target: messageListView.model

            onEventAdded:
            {
                messageListView.positionViewAtEnd()
            }
        }

        delegate: Item
        {
            id: msgItem

            width: messageListView.width
            height: Math.max(userImg.height, msgText.height) + 10

            MouseArea
            {
                anchors.fill: parent
                onClicked:
                {
                    tableModel.currentParticipantInView = model.event.user.username
                }
            }

            Rectangle
            {
                id: imgRect
                visible: !model.event.message.isFromMe && model.event.type === ChatEvent.MESSAGE
                property int imgSize: 50

                anchors.left: parent.left
                anchors.leftMargin: 5
                anchors.verticalCenter: parent.verticalCenter

                width: imgSize
                height: visible ? imgSize : 1
                radius: height / 2
                color: "transparent"

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        x: imgRect.x; y: imgRect.y
                        width: imgRect.width
                        height: imgRect.height
                        radius: imgRect.radius
                    }
                }

                Image
                {
                    id: userImg

                    anchors.centerIn: parent

                    source: model.image.photo
                    width: imgRect.imgSize
                    height: visible ? imgRect.imgSize : 1
                    sourceSize.width: width
                    sourceSize.height: height
                }
            }

            Text
            {
                id: msgText

                visible: model.event.type === ChatEvent.MESSAGE
                anchors.top: parent.top
                anchors.topMargin: 5

                text: model.event.message.message
                color: "white"
                width: messageListView.width * 0.65
                elide: Text.ElideRight
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere

                style: Text.Outline
                styleColor: "black"

                Component.onCompleted:
                {
                    if (!model.event.message.isFromMe)
                    {
                        anchors.left = imgRect.right
                        anchors.leftMargin = 5
                        horizontalAlignment = Text.AlignLeft
                    }
                    else
                    {
                        anchors.right = timeText.left
                        anchors.rightMargin = 5
                        horizontalAlignment = Text.AlignRight
                    }
                }
            }

            Text
            {
                id: timeText

                visible: model.event.type === ChatEvent.MESSAGE
                anchors.bottom: parent.bottom

                text: model.event.timeString
                color: "lightgray"
                width: 100
                style: Text.Outline
                styleColor: "black"

                Component.onCompleted:
                {
                    if (!model.event.message.isFromMe)
                    {
                        anchors.left = imgRect.right
                        anchors.leftMargin = 5 + msgText.paintedWidth + 5
                        horizontalAlignment = Text.AlignLeft
                    }
                    else
                    {
                        anchors.right = msgItem.right
                        anchors.rightMargin = 5
                        horizontalAlignment = Text.AlignLeft
                    }
                }

            }

            Text
            {
                id: eventText

                visible: model.event.type !== ChatEvent.MESSAGE
                anchors.top: parent.top
                anchors.topMargin: 5
                anchors.horizontalCenter: parent.horizontalCenter

                text:
                {
                    switch(model.event.type)
                    {
                    case ChatEvent.JOINED:
                    {
                        return "New user sat down"
                    }
                    case ChatEvent.LEFT:
                    {
                        return "User stood up"
                    }
                    case ChatEvent.STARTED:
                    {
                       return "Table was started"
                    }
                    case ChatEvent.END:
                    {
                        return "Table was ended"
                    }
                    }
                    return "Undefined message type"
                }

                color: "white"
                width: messageListView.width * 0.65
                horizontalAlignment: Text.AlignHCenter
                elide: Text.ElideRight
                wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }
        }
    }

    TextField
    {
        id: msgInput
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: msgSendButton.left
        height: 50

        onAccepted:
        {
            sendMessage(msgInput.text)
            msgInput.text = ""
        }
    }

    T.Button
    {
        id: msgSendButton
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        width: 50
        height: 50

        Image
        {
            anchors.fill: parent
            sourceSize.width: width
            sourceSize.height: height
            source: "qrc:/send_message.svg"
        }

        onClicked:
        {
            sendMessage(msgInput.text)
            msgInput.text = ""
        }
    }

    function sendMessage(text)
    {
        if (text.trim() === "")
            return;
        console.log("Sending text: " + text)
        tableModel.send(text)
    }
}

