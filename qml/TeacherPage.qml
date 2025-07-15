import QtQuick 2.2
import Sailfish.Silica 1.0

Page {
    id: teacherPage
    allowedOrientations: Orientation.All

    Column {
        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
            margins: Theme.paddingLarge
        }
        spacing: Theme.paddingLarge

        Label {
            text: qsTr("Режим преподавателя")
            font.pixelSize: Theme.fontSizeLarge
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
        }

        Button {
            id: rollButton
            width: parent.width
            text: bleScanner.scanning ? qsTr("Остановить перекличку") : qsTr("Начать перекличку")
            onClicked: {
                if (bleScanner.scanning)
                    bleScanner.stopRollCall()
                else
                    bleScanner.startRollCall()
            }
        }

        ProgressBar {
            id: progress
            width: parent.width
            minimumValue: 0
            maximumValue: 1
            value: bleScanner.scanning ? 0.5 : 0        // просто визуалка
            visible: bleScanner.scanning
        }

        Label {
            id: countLabel
            text: qsTr("Отметились: %1").arg(bleScanner.present.length)
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            visible: !bleScanner.scanning && bleScanner.present.length > 0
        }

        ListView {
            id: list
            width: parent.width
            height: parent.height - countLabel.height - progress.height - rollButton.height - 4*Theme.paddingLarge
            model: bleScanner.present
            delegate: ListItem {
                Label {
                    text: (index + 1) + ". " + modelData
                    anchors.verticalCenter: parent.verticalCenter
                }
            }
            visible: bleScanner.present.length > 0
        }

        // Инфо, если ещё нет студентов
        Label {
            text: qsTr("Список пуст")
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            visible: bleScanner.present.length === 0 && !bleScanner.scanning
        }
    }
}
