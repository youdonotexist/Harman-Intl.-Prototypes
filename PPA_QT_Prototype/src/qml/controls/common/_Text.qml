import QtQuick 2.0

Item { //TODO: Fix this so that the text alignment works with parent
    property string text

    Text {
        id: label
        anchors.fill: parent
        text: "123"
        color: "white"
        font.family: "Helvetica"
        //font.pointSize: parent.width * 0.1
        //width: parent.width
    }

    onTextChanged: {
        label.text = text;
    }
}
