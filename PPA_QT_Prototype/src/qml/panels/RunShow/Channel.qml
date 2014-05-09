import QtQuick 2.0

import "../../controls/common/peqgraph"
import "../../controls/common/dyncontroller"
import "../../controls/common"

Rectangle {
    id: channelRoot

    property string channelLabel
    property int index //TODO: put this on the container

    width: 150
    height: parent.height
    color: "black"

    Column {
        width: parent.width
        height: parent.height

        Rectangle { //Visualizations
            id: visParent
            width: parent.width
            height: parent.height * .30
            color: "black"
            border.width: 1
            border.color:"#263636"

            Column {
                id: vizContainer
                width: parent.width
                height: parent.height
                spacing: 5

                Rectangle {
                    height: (parent.height * .20) - 5
                    width: parent.width
                    radius: 2
                    color: "transparent"
                    border.color: "#041A09"
                    border.width: 1
                }

                DynamicControllerView {
                    width: parent.width
                    height: (parent.height * .30) - 5
                    simple: true
                    type: 4
                }

                PEQGraphView {
                    width: parent.width
                    height: (parent.height * .30) - 5
                    simple: true
                    horizontalDivisions: width

                    MouseArea {
                        anchors.fill: parent

                        function onDown(mouse) {
                            layer1.children[0].channelView = channelBottom;
                            layer1.children[0].dizzle();
                        }

                        onClicked: { this.onDown(mouse) }
                    }
                }

                Fader {
                    width: parent.width
                    height: (parent.height * .20) - 5

                    tickColor: "#00B242"
                    tickBackground: "#03260D"
                    orientation: 1
                    simple: true
                }

                Component {
                    id: peqComponent
                    PEQGraphView {
                        width: parent.width
                        height: (parent.height * .30) - 5
                        simple: true
                        horizontalDivisions: width

                        MouseArea {
                            anchors.fill: parent

                            function onDown(mouse) {
                                layer1.children[0].channelView = channelBottom;
                                layer1.children[0].dizzle();
                            }

                            onClicked: { this.onDown(mouse) }
                        }
                    }
                }
            }
        }
        Rectangle { //Slider
            id: channelBottom
            width: parent.width
            height: parent.height * .70

            color: "#263636"

            Column {
                width: parent.width
                height: parent.height
                anchors.top: parent.top
                anchors.topMargin: 10

                Rectangle { //Channel label
                    width: parent.width
                    height: parent.height * 0.08

                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 1
                    anchors.rightMargin: 1

                    color: "black"
                    radius: 2

                    Text {
                        height: parent.height
                        width: parent.width

                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter

                        color: "white"
                        font.pixelSize: parent.height * 0.4

                        text: channelLabel
                    }
                }

                TextButton {
                    height: parent.height * .25
                    width: parent.width

                    textEnabled: "ON"
                    textDisabled: "OFF"

                    textColorEnabled: "#00FA5D"
                    borderColorEnabled: "#00FA5D"
                    backgroundColorEnabled: "black"

                    textColorDisabled: "grey"
                    borderColorDisabled: "grey"
                    backgroundColorDisabled: "black"
                }

                Row { //Fader
                    width: parent.width - 10
                    height: parent.height * .60

                    anchors.left: parent.left
                    anchors.leftMargin: 5
                    anchors.rightMargin: 5

                    Rectangle { //fader coloring
                        width: parent.width * .15
                        height: parent.height

                        border.color: "black"
                        border.width: 2
                        radius: 2
                        gradient: Gradient {

                                GradientStop { position: 0.0; color: "#FF143D" }
                                GradientStop { position: 0.10; color: "#F2FF58" }
                                GradientStop { position: 1.0; color: "#00B442" }
                            }
                    }

                    Fader {
                        width: parent.width * .85
                        height: parent.height
                        tickColor: "#6B6D6C"
                        tickBackground: "black"
                    }
                }
            }
        }
    }
}
