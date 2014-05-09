import QtQuick 2.0
import HIControls 1.0

Rectangle {
    id: parent

    property alias type: root.type
    property alias simple: root.simple

    width: parent.width
    height: parent.height
    clip: true
    color:"transparent"

    DynamicController {
        id: root
        width: simple ? parent.width : parent.width * .90
        height: simple ? parent.height : parent.height * .90

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        color: "#03260D"
        primaryLineColor: "#00F35A"
        curveColor: "#00AB3F"
        simple: false

        Rectangle {
            width: root.simple ? parent.width : parent.width * .90
            height: root.simple ? parent.height : parent.height * .90
            color: "#041A09"
            border.color: root.primaryLineColor
            border.width: 1
            radius: 2
            z: -1
        }

    }
}
