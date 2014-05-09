var knobs = [];
var tempKnob;

/**
  * We used to draw all the knobs at init, but looking at the PA2 implemention, they created their knobs dynamically on touch
  *
  */
function drawKnobs() {
    var component = Qt.createComponent("../controls/common/Knob.qml");

    for (var i = 0; i < 8; i++) {
        var pos = root.getKnobGraphPosition(i)
        var obj = component.createObject (
                root,
                {
                   x:pos.x,
                   y:pos.y,
                   index: i
                }
         );
    }
}

/*
  * Creates a knob dynamically. Takes position of the knob as a paramter.
  *
  * Also connects the knob's knobmove signal to a slot in the graph
  */
function drawKnob(position) {
    if (knobs.length < root.channelCount()) {
        var component = Qt.createComponent("../controls/common/Knob.qml"),
            index = knobs.length,
            obj = component.createObject (
                root,
                {
                   x:position.x,
                   y:position.y,
                   index: index
                }
            );

        knobs.push(obj);
        tempKnob = obj;

        obj.knobmove.connect(root.setKnobGraphPosition);
        root.setKnobGraphPosition(index, position.x, position.y);
        root.onKnobCountChanged();
    }
}

function updateValue(index, type, amount) {
    if (type === "gain") {
        root.onGainChanged(index, amount);
    }
    else if (type === "q") {
        root.onQChanged(index, amount);
    }
    else if (type === "freq") {
        root.onFreqChanged(index, amount);
    }
}
