import QtQuick 2.9
import QtQuick.Templates 2.2 as T
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import QtGraphicalEffects 1.0
import QtQuick.Controls.Styles 1.4
import models 1.0
import enums 1.0

Item{

    Column {
        anchors.centerIn: parent

        Text {
            text: "Enter your username"
            color: "lightblue"
        }

        TextField
        {
            id: usernameInput
            height: 50

            onAccepted:
            {
                if (text.length > 4) {
                    stackView.push("qrc:/ChatScreen.qml", {username: text})
                }
            }
        }
    }
}
