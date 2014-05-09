import QtQuick 2.0

Rectangle {
    height: parent.height
    width: parent.width
    color: "orange"
    Row {
        anchors.verticalCenter: parent.verticalCenter
        Rectangle {
            radius: 3
            width: 50
            height: 50
        }
    }
}
