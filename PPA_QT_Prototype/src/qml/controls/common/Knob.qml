import QtQuick 2.0

Rectangle {
    id: container

    property int index
    property var borderColor

    signal knobmove(int index, double x, double y)

    width: parent ? Math.max(44, Math.min(parent.width, parent.height) * 0.1)  : 0
    height: parent ? Math.max(44, Math.min(parent.width, parent.height)  * 0.1) : 0
    color: "transparent"
    objectName: "knob"
    borderColor: "#00FF5F"

    Rectangle {
        anchors {
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
            horizontalCenterOffset: -parent.width * 0.5
            verticalCenterOffset: -parent.height * 0.5
        }
        border {
            width: 2
            color: borderColor
        }
        width: parent.width
        height: parent.height
        color: Qt.darker(borderColor)
        radius: Math.min(parent.width, parent.height) * 0.5

        Text {
            text: index
            visible: false
            anchors {
                horizontalCenter: parent.horizontalCenter
                verticalCenter: parent.verticalCenter
            }
            //font.pointSize: parent.height * .20
        }

        MouseArea {
            id: mouseArea
            width: parent.width
            height: parent.height
            propagateComposedEvents: true

            function onPositionUpdate(mouse) {
                var globalPos = container.parent.mapFromItem(mouseArea, mouse.x, mouse.y);
                knobmove(container.index, globalPos.x, globalPos.y);
            }

            onPressed: { this.onPositionUpdate(mouse) }
            onPositionChanged: { this.onPositionUpdate(mouse) }
            onClicked: {}
        }
    }
}
