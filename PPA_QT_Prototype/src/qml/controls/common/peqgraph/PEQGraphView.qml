import QtQuick 2.1
import HIControls 1.0
import "../../../js/graphScripts.js" as GraphScripts

Rectangle {
    id: parent
    width: parent.width
    height: parent.height
    clip: true
    color:"transparent"

    property alias simple: root.simple
    property alias horizontalDivisions:root.horizontalDivisions

    PEQGraph {
        id: root

        width: root.simple ? parent.width : parent.width * .90
        height: root.simple ? parent.height : parent.height * .90

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        color: "#03260D"
        primaryLineColor: "#00F35A"
        horizontalDivisions: width
        curveColor: "#00AB3F"
        simple: false

        signal updateValue(int index, string type, double amount)

        function onKnobMove(container, pos){
            root.setKnobGraphPosition(container.index, pos.x, pos.y);
        }

        //TODO: Remove debug code
        /*Timer {
            interval: 100
            running: false //root.visible
            repeat: true
            onTriggered: randomize()
        }

        //TODO: Remove debug code
        function randomize() {
            if (root.visible) {
                for (var i = 0; i < 1; i++) {
                    GraphScripts.updateValue(i, "gain", (Math.random() * 30) - 15);
                    GraphScripts.updateValue(i, "freq", (Math.random() * 20000));
                }
            }
        }*/

        Rectangle {
            width: root.simple ? parent.width : parent.width * .90
            height: root.simple ? parent.height : parent.height * .90
            color: "#041A09"
            border.color: root.primaryLineColor
            border.width: simple ? 1 : 0
            radius: 2
            z: -10
        }

        MouseArea {
            width: parent.width
            height: parent.height

            onPressed: root.onGraphPressed(mouse)
            onPositionChanged: root.onGraphKnobDrag(mouse)
            onReleased: { GraphScripts.tempKnob = null }
        }

    /**
      * When we're creating a knob for the first time, the graph is forced to handle the drag/movement
      * since Qt doesn't detect when an object is instantiated under the users finger.
      */
        function onGraphPressed(mouse) {
            if (!root.simple) {
                GraphScripts.drawKnob({x: mouse.x, y: mouse.y})
            }
        }

        function onGraphKnobDrag(mouse) {
             if (!root.simple) {
                 if (GraphScripts.tempKnob) {
                     root.onKnobMove(GraphScripts.tempKnob, {x: mouse.x, y: mouse.y});
                 }
             }
        }

        onUpdateValue: {
            GraphScripts.updateValue(index, type, amount);
        }
    }
}
