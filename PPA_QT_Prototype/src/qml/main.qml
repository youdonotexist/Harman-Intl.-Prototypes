import QtQuick 2.1
import QtGraphicalEffects 1.0
import HIControls 1.0

import 'panels/RunShow'

Rectangle {
    id: window
    width: 1024
    height: 768
    color: "black"
    MainMenu {
        id: mainMenu
        height: parent.height * 0.05
        width: parent.width
    }

    Rectangle {
        id: rootContainer
        anchors.top: mainMenu.bottom
        color: "yellow"
        height: parent.height * 0.95
        width: parent.width
        anchors.topMargin: 10

        RunShowPanel {
            visible: mainMenu.currentState == "RunShowPanel"
        }

        Rectangle {
            anchors.fill: parent;
            visible: mainMenu.currentState == "Home"
            color: "blue"
            NewControl {
                id: cosmo
                anchors.fill: parent
                radius: 100

                onRadChanged: console.log("This Happened")

                MouseArea {
                    anchors.fill: parent
                    onPositionChanged: cosmo.updateRadius(mouse.x, mouse.y)
                }
            }
        }

        Rectangle {
            anchors.fill: parent;
            visible: mainMenu.currentState == "ParametricEQTest"
            color: "yellow"
        }
    }

    FPSMonitor {
        width: 200
        height: 100

        enabled: true

        anchors.top: parent.top
        anchors.right: parent.right

        z: 1000
    }

    DropShadow {
        height: parent.height
        width: parent.width * 2
        transparentBorder: true
        verticalOffset: parent.height * 0.002
        fast: true
        radius: 8.0
        samples: 16
        source: mainMenu
   }
}
