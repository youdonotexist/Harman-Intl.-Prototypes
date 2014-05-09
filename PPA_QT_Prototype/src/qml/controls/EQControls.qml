import QtQuick 2.0

import "common"

Column {
    property var graphDelegate
    property int channelIndex

    id: root
    width: parent.width / 4.0
    height: parent.height

    Component.onCompleted: {
        gain.buildDial(-20, 20, 0, 0.5);
        freq.buildDial(20, 20000, 0, 1.0);
        q.buildDial(0.01, 50, 40, 0.2);

        graphDelegate.requestUpdatedValues(channelIndex);
    }

    function handleValueChanged(channel, type, amount) {
        if (type === "gain") {
            gain.updateValue(amount);
        }
        else if (type === "q") {
            q.updateValue(amount);
        }
        else if (type === "freq") {
            freq.updateValue(amount);
        }
    }

    EQControl {
        id: gain
        channelIndex: root.channelIndex
        displayName: "Gain"
        valueId: "gain"
    }

    EQControl {
        id: q
        channelIndex: root.channelIndex
        displayName: "Q"
        valueId: "q"
    }

    EQControl {
        id: freq
        channelIndex: root.channelIndex
        displayName: "Freq"
        valueId: "freq"
    }
}
