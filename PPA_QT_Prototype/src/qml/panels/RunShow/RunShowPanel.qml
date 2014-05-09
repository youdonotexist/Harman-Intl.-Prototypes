import QtQuick 2.1
import QtQuick.Controls 1.1

import "../../controls"
import ".."

Rectangle {
    width: parent.width
    height: parent.height
    color: "black"

    Item {
        id: layer0
        width: parent.width
        height: parent.height

        Column { // Bank Manager, Channel Manager
            width: parent.width
            height: parent.height
            Item { //Bank Manager
                id: bankManager
                width: parent.width
                height: parent.height * .10

                Flickable {
                    id: scrollRoot
                    width: parent.width
                    height: parent.height

                    Item {
                        width: (bankManager.width * .09) * 26
                        height: parent.parent.height
                        Row {
                            id: bankContainer
                            width: parent.width
                            height: parent.height

                            Repeater {
                                model: 26
                                Column {
                                    property string textValue: index;

                                    height: bankManager.height  * 0.5
                                    width: bankManager.width * .09
                                    Text {
                                        id: bankText
                                        width: parent.width
                                        height: parent.height * 0.5
                                        horizontalAlignment: Text.AlignHCenter
                                        color: "white"
                                        text: textValue

                                        font.pixelSize: parent.height * 0.4
                                    }

                                    Image {
                                        source: "../../images/groups.png"
                                        width: parent.width * 0.8
                                        height: parent.height * 0.5

                                        fillMode: Image.PreserveAspectFit
                                        horizontalAlignment: Image.AlignHCenter
                                        verticalAlignment: Image.AlignVCenter
                                    }
                                }
                            }

                            Component.onCompleted: {
                                scrollRoot.contentWidth = bankContainer.width
                                scrollRoot.contentHeight = parent.height
                            }
                        }
                    }

                    anchors.fill: parent
                }
            }

            Rectangle { //Channel manager
                id: channelManager

                height: parent.height * .90
                width: parent.width
                color: "black"

                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 10
                anchors.rightMargin: 10

                clip: true

                Component {
                    id: channelComponent
                    Channel {
                        color: "black"
                    }
                }

                HInfiniteScrollView {
                    id: scrollView

                    width: parent.width
                    height: parent.height

                    delegate: scrollView
                    viewSpacing: scrollView.width * 0.005
                    componentTemplate: channelComponent
                    fitToScreenCount:8

                    function numberOfViews() {
                        return 100;
                    }

                    function configForIndex(index) {
                        var ct = scrollView.fitToScreenCount - 1;
                        return {
                            width: Qt.binding(function() {
                                return -(((scrollView.viewSpacing / ct) - scrollView.width) / (ct + 1))
                            }),
                            height: Qt.binding(function() { return scrollView.height }),
                            channelLabel: index + 1
                        };
                    }

                    function widthOfViewAtIndex(index) {
                        var ct = scrollView.fitToScreenCount - 1;
                        return -(((scrollView.viewSpacing / ct) - scrollView.width) / (ct + 1))
                    }
                }
            }
        }
    }

    Item {
        anchors.fill: parent
        id: layer1

        DetailPanelTest {
            id: detailPanel
            anchors.fill: parent

        }
    }
}

