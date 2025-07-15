import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: studentPage
    allowedOrientations: Orientation.All

    Column {
        anchors.centerIn: parent
        spacing: Theme.paddingLarge

        Label {
            text: qsTr("Режим студента")
            font.pixelSize: Theme.fontSizeLarge
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }

        TextField {
            id: idField
            width: parent.width * 0.8
            placeholderText: qsTr("Введите идентификатор")
            inputMethodHints: Qt.ImhDigitsOnly
        }

        Button {
            id: sendBtn
            text: qsTr("Отметиться")
            enabled: idField.text.length > 0 && !busy.running

            onClicked: {
                bleAdvertiser.advertiseStudent(idField.text)
                busy.running = true
            }
        }

        BusyIndicator {
            id: busy
            running: false
            size: BusyIndicatorSize.Medium
        }

        Timer {                    // авто-сброс Busy после 10 с
            running: busy.running
            interval: 10000
            onTriggered: busy.running = false
        }

        Label {
            text: busy.running ? qsTr("Отправляется…") : ""
            color: Theme.highlightColor
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }
    }
}
