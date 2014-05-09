import QtQuick 2.0

Rectangle {
    property string centerText
    property int channelIndex
    property string displayName
    property string valueId

    border.width: 1
    border.color: "black"
    color: "green"
    width: parent.width
    height: parent.height / 4.0

    Text {
        anchors.fill: parent
        text: displayName
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    MouseArea {
        width: parent.width
        height: parent.height
        onPressed: {
            parent.z = 5
            dial.visible = true
            dial.touchDown(mouse)
        }
        onPositionChanged: {
            dial.touchMoved(mouse)
        }
        onReleased: {
            parent.z = 0
            dial.visible = false
        }
    }

    function buildDial(minValue, maxValue, startValue, degreeRatio) {
        dial.minValue = minValue;
        dial.maxValue = maxValue;
        dial.currentValue = startValue;
        dial.degreeToIncrement = degreeRatio;
    }

    function updateValue(value) {
        dial.currentValue = value;
        dial.centerText = Number(value).toFixed(2)
    }

    Dial {
        id: dial
        visible: false
        centerText: Number(parent.centerText).toFixed(2)
        onValueChanged: {
            graphDelegate.updateValue(channelIndex, valueId, amount);
        }
    }
}
