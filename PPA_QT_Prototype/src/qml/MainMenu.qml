import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Controls.Styles 1.0
import QtGraphicalEffects 1.0
import "js/homeScripts.js" as HomeScript

Rectangle {
    id: mainMenu

    property string currentState;

    anchors.top: parent.top
    color: "black"
    Row {
        id: menuContainer
        anchors.left: parent.left
        height: parent.height

        Component {
            id: menuButtonComponent
            Button {
                property string buttonText
                property string buttonAction
                height: parent.height
                width: 100
                activeFocusOnPress: true
                onClicked: {
                    mainMenu.currentState = this.buttonAction;
                }
                style: ButtonStyle {
                    background: Rectangle {
                        color: "transparent"
                        Text {
                            id: temp
                            text: buttonText
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            width: parent.width
                            height: parent.height
                            color: control.activeFocus ? "white" : "darkgrey"
                            font.pixelSize: parent.height * 0.4
                        }
                    }
                }
            }
        }

        Component.onCompleted: {
            var buttons = [
                {
                    text: "Find\nDevices",
                    action: "Home"
                },
                {
                    text: "Patch\nOutputs",
                    action: "ParametricEQTest"
                },
                {
                    text: "Run\nShows",
                    action: "RunShowPanel"
                },

            ]

            for (var i = 0; i < buttons.length; i++) {
                var button = buttons[i];
                var buttonObj = menuButtonComponent.createObject(menuContainer);
                buttonObj.buttonText = button.text;
                buttonObj.buttonAction = button.action;
            }
        }

        Rectangle {
            color: "lightgrey"
            width: 1
            height: mainMenu.height
        }
    }
}
