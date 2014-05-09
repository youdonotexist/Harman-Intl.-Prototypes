import QtQuick 2.0

Rectangle { //Enable Toggle
    id: root
    property string buttonText
    property color textColor
    property color borderColor
    property color backgroundColor

    property string textEnabled
    property color textColorEnabled
    property color borderColorEnabled
    property color backgroundColorEnabled

    property string textDisabled
    property color textColorDisabled
    property color borderColorDisabled
    property color backgroundColorDisabled

    property bool toggle: true
    property bool buttonUp: true

    states: [
        State {
            name: "up"; when: buttonUp == true
            PropertyChanges {
                target: root;
                buttonText: textEnabled
                textColor: textColorEnabled
                borderColor: borderColorEnabled
                backgroundColor: backgroundColorEnabled
            }
        },

        State {
            name: "down"; when: buttonUp == false
            PropertyChanges {
                target: root;
                buttonText: textDisabled
                textColor: textColorDisabled
                borderColor: borderColorDisabled
                backgroundColor: backgroundColorDisabled
            }
        }
    ]

    width: parent.width
    height: parent.height * .25
    color: "transparent"
    Rectangle {
        height: parent.height * .30
        width: parent.width

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        color: backgroundColor
        border.color: borderColor
        radius: 2

        Text {
            anchors.fill: parent
            text: buttonText
            font.bold: true
            font.pixelSize: parent.height * 0.4
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            color: textColor
        }

        MouseArea {
            width: parent.width
            height: parent.height
            onClicked: {
                if (root.toggle) {
                    root.buttonUp = !root.buttonUp;
                } else {

                }
            }

            onPressed: {
                if (!root.toggle) {

                }
            }
        }
    }
}
