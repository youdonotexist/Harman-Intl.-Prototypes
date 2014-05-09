import QtQuick 2.0
Item {
    id: root
    property double firstAngle
    property string centerText

    property double maxValue
    property double minValue
    property double currentValue

    property double degreeToIncrement

    signal valueChanged(var control, double amount)

    width: 100
    height: 100

    function r2d(rad) {
        return rad * (180.0/Math.PI);
    }

    function sign (number) {
        return number ? number < 0 ? -1 : 1 : 0
    }

    function touchDown(mouse) {
        this.firstAngle = r2d(Math.atan2(mouse.y + (dialImage.height * 0.5), mouse.x + (dialImage.width * 0.5)));
    }

    function touchMoved(mouse) {
        var secondAngle = r2d(Math.atan2(mouse.y - (dialImage.height * 0.5), mouse.x - (dialImage.width * 0.5)));
        var angle = secondAngle - this.firstAngle;

        if (Math.abs(angle) > 1.0) {
            var end = sign(angle) * root.degreeToIncrement + root.currentValue;
            end = Math.max(minValue, end);
            end = Math.min(maxValue, end);
            dialImage.rotation += angle;
            root.valueChanged(root, end);
            this.firstAngle = secondAngle;
         }
    }

    Image {
        id: dialImage
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.fill: parent
        source: "images/WheelRotate.png"

    }

    Text {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        text: centerText
    }

    MouseArea {
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        width: 10000
        height: 10000

        onPressed: parent.touchDown(mouse)
        onPositionChanged: parent.touchMoved(mouse)

    }

}
