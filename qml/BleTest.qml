import QtQuick 2.0
import Sailfish.Silica 1.0

ApplicationWindow {
    id: appWindow

    /* первая страница — выбор роли */
    initialPage: Component {
        Page {
            Column {
                anchors.centerIn: parent
                spacing: Theme.paddingLarge

                Label {
                    text: qsTr("Выберите режим")
                    horizontalAlignment: Text.AlignHCenter
                    font.pixelSize: Theme.fontSizeLarge
                }

                Button {
                    text: qsTr("Преподаватель")
                    onClicked: pageStack.push(Qt.resolvedUrl("TeacherPage.qml"))
                }

                Button {
                    text: qsTr("Студент")
                    onClicked: pageStack.push(Qt.resolvedUrl("StudentPage.qml"))
                }
            }
        }
    }
}
