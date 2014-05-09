import QtQuick 2.0

Rectangle {
    id: root

    property double degreeToIncrement: 0.1
    property double minValue: 0
    property double maxValue: 1
    property double currentValue: 0

    color: "transparent"

    QtObject {
        id: internal
        property double _firstAngle
        property double _startAngle: 200
        property double _endAngle: 160
        property double _minAngleNormalized: 360 / _startAngle
        property double _maxAngleNormalized: 360 / _endAngle

        property double _initAngle: 0
    }

    Component.onCompleted: {

        //So that we don't get a discontinuty crossing over 360 => 0 threshold
        if (internal._startAngle > internal._endAngle ) {
            internal._startAngle -= 360;
        }
    }

    Rectangle {
        id: dial
        width: parent.width * 0.5
        height: parent.height * 0.5
        radius: width * 0.5

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        border.width: width * 0.05
        border.color: "green"
        color: Qt.darker("green")

        clip: true

        Rectangle {
           width: parent.width * .20
           height: parent.height

           anchors.horizontalCenter: parent.horizontalCenter
           anchors.bottom: parent.verticalCenter

           radius: height * 0.5

           color: "black"

           border.width: parent.width * 0.05
           border.color: "green"
        }
   }

    MouseArea {
        id: mouseArea
        anchors.fill: parent

        onPressed: parent.touchDown(mouse)
        onPositionChanged: parent.touchMoved(mouse)
    }

    function touchDown(mouse) {
        var yDelta = mouse.y - (mouseArea.height * 0.5),
            xDelta =  mouse.x - (mouseArea.width * 0.5);
        internal._firstAngle = calculateAngle(xDelta, yDelta);
    }

    function touchMoved(mouse) {
        var yDelta = mouse.y - (mouseArea.height * 0.5),
            xDelta =  mouse.x - (mouseArea.width * 0.5);

        var secondAngle = calculateAngle(xDelta, yDelta),
            deltaAngle;

        //Special cases where angles fall across atan2 discontinutity
        // If our first angle is in quadrant 1 and our second angle is in quadrant 4
        if ((internal._firstAngle >= 270 && internal._firstAngle <= 360) && (secondAngle >= 0.0 && secondAngle <= 90.0)) {
            //Transform the second angle into the first
            deltaAngle = internal._firstAngle - (secondAngle + 360);
        }
        // If our first angle is in quadrant 4 and our second angle is in quadrant 1
        else if ((internal._firstAngle >= 0 && internal._firstAngle <= 90) && (secondAngle >= 270 && secondAngle <= 360)) {
            deltaAngle = (internal._firstAngle + 360) - secondAngle;
        }
        else { //Just calculate the angle difference normally
            deltaAngle = internal._firstAngle - secondAngle;
        }

        var testRotation = dial.rotation - deltaAngle,
            normalizedRotation = rangeAngle(testRotation);

        //Snap to our min/max angles
        testRotation = Math.max(internal._startAngle, testRotation);
        testRotation = Math.min(internal._endAngle, testRotation);

        dial.rotation = testRotation;
        internal._firstAngle = secondAngle;

        currentValue = (testRotation - internal._startAngle) / (internal._endAngle - internal._startAngle);;
    }

    function calculateAngle(xDelta, yDelta) {
        var a = Math.atan2(yDelta, xDelta);

        if (a<0)
        {
            a = 2*Math.PI + a;
        }

        return r2d(a);
    }

    function rangeAngle(angle) {
        var ret = angle;
        if (angle > 0) {
            if (angle >= 360) {
                ret -= 360;
            }
        }
        else if (angle < 0) {
            ret += 360;
        }

        return ret;
    }

    function r2d(rad) {
        return rad * (180.0/Math.PI);
    }

    function sign (number) {
        return number ? number < 0 ? -1 : 1 : 0
    }
}
