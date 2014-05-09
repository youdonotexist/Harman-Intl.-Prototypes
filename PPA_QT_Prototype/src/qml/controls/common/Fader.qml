import QtQuick 2.0
import QtGraphicalEffects 1.0

Rectangle { //Fader control
    id: sliderContainer

    //0: vertical, 1: horizontal
    property int orientation: 0

    property color tickColor
    property color tickBackground
    property bool simple: false

    color: "transparent"

    Component {
        id: knobComponent
        Rectangle {
            id: slider
            width:orientation == 0 ? parent.width * .80 : parent.width * .05
            height: orientation == 0 ? parent.height * .20 : (simple ? parent.height : parent.height * 1.10)
            y: orientation == 0 ? -sliderContainer.height * .10 : 0;
            x: orientation == 0 ? 0 : -slider.width * .025

            anchors.horizontalCenter: orientation == 0 ? parent.horizontalCenter : undefined
            anchors.verticalCenter: orientation == 0 ? undefined : parent.verticalCenter

            color: "transparent"
            z:2

            DropShadow {
                anchors.fill: sliderVisual
                transparentBorder: true
                verticalOffset: 5
                horizontalOffset: 5
                fast: true
                radius: 8.0
                samples: 16
                source: sliderVisual
                visible: !simple
            }


            Rectangle {
                id: sliderVisual
                anchors.fill: parent
                border.color: Qt.lighter(tickColor, 1.8)
                border.width: 2
                radius: 5
                color: tickColor

                Rectangle {
                    width: orientation == 0 ? parent.width * .80 : parent.width * .25
                    height: orientation == 0 ? parent.height * .15 : parent.height * .80

                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter

                    border.color: Qt.lighter(tickColor, 1.8)
                    border.width: 2
                    color: tickBackground
                    radius: 5
                }
            }

            MouseArea {
                anchors.fill: parent
                drag.target: parent
                drag.axis: orientation == 0 ? Drag.YAxis : Drag.XAxis

                drag.minimumX: orientation == 1 ? -sliderContainer.width * .025 : -1000000;
                drag.maximumX: orientation == 1 ? sliderContainer.width - (sliderContainer.width * .025) : 1000000;

                drag.minimumY: orientation == 0 ? -sliderContainer.height * .10 : -100000;
                drag.maximumY: orientation == 0 ? sliderContainer.height - (sliderContainer.height * .10) : 100000;

                enabled: !simple

            }
        }
    }

    Component {
        id: verticalSegments

        Item {
            property int tickCount: 11
            width: parent.width
            height: parent.height

            Column {
                width: parent.width
                height: parent.height
                spacing: (parent.height * (1/(tickCount-1))) - (parent.height * 0.01) - (parent.height * 0.01 * (1/(tickCount - 1)))

                Repeater {
                    model: tickCount
                    Rectangle {width:parent.width*.90;anchors.horizontalCenter: parent.horizontalCenter;height:parent.height*0.01;color:tickColor}
                }
            }

            Rectangle {
                width: parent.width * 0.1
                height: parent.height + 5

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                color: tickBackground
            }
        }
    }

    Component {
        id: horizontalSegments
        Item {
            property int tickCount: 7
            width: parent.width
            height: simple ? parent.height : parent.height * 0.5
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter

            Rectangle {
                width: parent.width
                height: parent.height

                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                color: tickBackground
            }

            Row {
                height: parent.height

                //First, get the width of each segment as if it was spaced equally across the parent
                //Subtract the width of one tick
                //Then subtract 1/10th from the spacing of each tick so that the last tick is inside the bounding box
                spacing: (parent.width * (1/(tickCount-1))) - (parent.width * 0.01) - (parent.width * 0.01 * (1/(tickCount - 1)))

                Repeater {
                    model: tickCount
                    Rectangle {
                        height:parent.height*.90;
                        anchors.verticalCenter:parent.verticalCenter;
                        width:parent.parent.width * 0.01;
                        color:index == 0 || index == tickCount-1 ? tickColor : Qt.darker(tickColor)
                    }
                }
            }
        }
    }

    Component.onCompleted: {
        if (orientation == 0) {
            verticalSegments.createObject(sliderContainer);
        }
        else {
            horizontalSegments.createObject(sliderContainer);
        }

        knobComponent.createObject(sliderContainer);

    }
}
