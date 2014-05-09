import QtQuick 2.1
import  HIControls 1.0

import "../controls/common"
import "../controls/common/peqgraph"
import "../controls/common/dyncontroller"

Rectangle {
    id: detailPanelRoot
    property var channelView
    property var leftBorder
    property var rightBorder
    property var bottomBorder
    property var mainTopBorder
    property var mainLeftBorder
    property var mainRightBorder
    property var mainBottomLeftBorder
    property var mainBottomRightBorder
    property var mainBackground;

    property var detailComponent;

    states: [
        State {
            name: "open"
            PropertyChanges {
                target: detailPanelRoot
                width: parent.width
            }
        }
    ]

    color: "transparent"
    MouseArea {
        id: mainMouseArea
        width: parent.width
        height: parent.height
        enabled: false
        z: -10
    }

    Component {
        id: highlightOutlineComponent
        Rectangle {
            color: "#00FF64"
        }
    }

    Component {
        id: highlightBackgroundComponent
        Rectangle {
            id: backgroundRoot
            color: "#030D04"

            Row {
                width: parent.width
                height: parent.height

                Rectangle {
                    id: options
                    width: parent.width * .10
                    height: parent.height
                    color: "transparent"
                }

                Rectangle {
                    id: details
                    width: parent.width * .80
                    height: parent.height
                    color: "transparent"

                    PEQGraphView {
                        width: parent.width * 0.8
                        height: parent.height * 0.8
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }

                    /**
                     * Uncomment these to enable the different detail views
                    **/

                    /*DynamicControllerView {
                        width: parent.width * 0.9
                        height: parent.height * 0.95
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                        type: DynamicController.GateCompressor
                    }*/

                    /*SimpleDial {
                        width: parent.height * 0.5
                        height: parent.height * 0.5
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter
                    }*/

                    /*Fader {
                        width: parent.width * 0.5
                        height: parent.height * 0.2
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.verticalCenter: parent.verticalCenter

                        tickColor: "#00B242"
                        tickBackground: "#03260D"
                        orientation: 1
                    }*/
                }

                Rectangle {
                    id: detailsControls
                    width: parent.width * .10
                    height: parent.height
                    color: "transparent"

                    Rectangle {
                        width: parent.width
                        height: parent.height
                        color: "transparent"
                        Text {
                            anchors.right: parent.right
                            anchors.top: parent.top
                            width: parent.width
                            text: "X"
                            font.pointSize: parent.width * .20
                            horizontalAlignment: Text.AlignRight
                            color: "grey"
                            anchors.rightMargin: width * .05
                            anchors.topMargin: height * .20

                            MouseArea {
                                width: parent.width
                                height: parent.height
                                onClicked: {
                                    detailPanelRoot.show(false);
                                }
                            }
                        }

                        Text {
                            text: "[  ]"

                            anchors.right: parent.right
                            anchors.bottom: parent.bottom
                            width: parent.width
                            color: "grey"
                            font.pointSize: parent.width * .20
                            horizontalAlignment: Text.AlignRight
                            anchors.rightMargin: width * .05
                            anchors.bottomMargin: height * .20
                        }

                    }
                }
            }
        }
    }

    Component.onCompleted: {
        //Left
        detailPanelRoot.leftBorder = highlightOutlineComponent.createObject(detailPanelRoot, {});
        detailPanelRoot.rightBorder = highlightOutlineComponent.createObject(detailPanelRoot, {});
        detailPanelRoot.bottomBorder = highlightOutlineComponent.createObject(detailPanelRoot, {});

        detailPanelRoot.mainTopBorder = highlightOutlineComponent.createObject(detailPanelRoot, {});
        detailPanelRoot.mainLeftBorder = highlightOutlineComponent.createObject(detailPanelRoot, {});
        detailPanelRoot.mainRightBorder = highlightOutlineComponent.createObject(detailPanelRoot, {});
        detailPanelRoot.mainBottomLeftBorder = highlightOutlineComponent.createObject(detailPanelRoot, {});
        detailPanelRoot.mainBottomRightBorder = highlightOutlineComponent.createObject(detailPanelRoot, {});

        detailPanelRoot.mainBackground = highlightBackgroundComponent.createObject(detailPanelRoot, {});
    }

    onWidthChanged: {
        dizzle();
    }

    onHeightChanged: {
        dizzle();
    }

    function dizzle() {
        if (channelView) {
            var mappedPosL = channelView.mapToItem(detailPanelRoot, 0, 0);
            var mappedPosR = channelView.mapToItem(detailPanelRoot, channelView.width, 0);
            var mappedPosB = channelView.mapToItem(detailPanelRoot, 0, channelView.height);

            detailPanelRoot.show(true);

            detailPanelRoot.leftBorder.x = mappedPosL.x;
            detailPanelRoot.leftBorder.y = mappedPosL.y;
            detailPanelRoot.leftBorder.width = 3;
            detailPanelRoot.leftBorder.height = channelView.height;

            detailPanelRoot.rightBorder.x = mappedPosR.x;
            detailPanelRoot.rightBorder.y = mappedPosR.y;
            detailPanelRoot.rightBorder.width = 3;
            detailPanelRoot.rightBorder.height = channelView.height;

            detailPanelRoot.bottomBorder.x = mappedPosB.x;
            detailPanelRoot.bottomBorder.y = mappedPosB.y - 13;
            detailPanelRoot.bottomBorder.height = 3;
            detailPanelRoot.bottomBorder.width = channelView.width;

            //Top
            detailPanelRoot.mainTopBorder.x = 0;
            detailPanelRoot.mainTopBorder.y = 0;
            detailPanelRoot.mainTopBorder.width = parent.width;
            detailPanelRoot.mainTopBorder.height = 3

            //Left
            detailPanelRoot.mainLeftBorder.x = 0;
            detailPanelRoot.mainLeftBorder.y = 3;
            detailPanelRoot.mainLeftBorder.height = mappedPosL.y;
            detailPanelRoot.mainLeftBorder.width = 3;

            //Right
            detailPanelRoot.mainRightBorder.x = parent.width - 3;
            detailPanelRoot.mainRightBorder.y = 0;
            detailPanelRoot.mainRightBorder.height = mappedPosL.y;
            detailPanelRoot.mainRightBorder.width = 3;

            //Bottom, Leftside
            detailPanelRoot.mainBottomLeftBorder.x = 0;
            detailPanelRoot.mainBottomLeftBorder.y = mappedPosL.y;
            detailPanelRoot.mainBottomLeftBorder.height = 3;
            detailPanelRoot.mainBottomLeftBorder.width = mappedPosL.x;

            //Bottom, Rightisde
            detailPanelRoot.mainBottomRightBorder.x = mappedPosR.x;
            detailPanelRoot.mainBottomRightBorder.y = mappedPosL.y;
            detailPanelRoot.mainBottomRightBorder.height = 3;
            detailPanelRoot.mainBottomRightBorder.width = parent.width;

            detailPanelRoot.mainBackground.x = 0;
            detailPanelRoot.mainBackground.y = 0;
            detailPanelRoot.mainBackground.width = detailPanelRoot.width;
            detailPanelRoot.mainBackground.height = mappedPosL.y;
            detailPanelRoot.mainBackground.z = -1;
        }
    }

    function show (yes) {
        if (detailPanelRoot.mainBackground) { //Check to see if we're completed
            detailPanelRoot.leftBorder.visible = yes;
            detailPanelRoot.rightBorder.visible = yes;
            detailPanelRoot.bottomBorder.visible = yes;

            detailPanelRoot.mainTopBorder.visible = yes;
            detailPanelRoot.mainLeftBorder.visible = yes;
            detailPanelRoot.mainRightBorder.visible = yes;
            detailPanelRoot.mainBottomLeftBorder.visible = yes;
            detailPanelRoot.mainBottomRightBorder.visible = yes;

            detailPanelRoot.mainBackground.visible = yes;

            mainMouseArea.enabled = yes;
        }
    }
}
