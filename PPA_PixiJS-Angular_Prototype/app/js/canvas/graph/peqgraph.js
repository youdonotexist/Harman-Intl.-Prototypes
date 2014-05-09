/**
 * Created by mdaly on 12/31/13.
 */
PEQGraph = function(width, height) {
    PEQGraph.superclass.call(this, width, height);
    this.CurveCalc = new CurveCalculator(1, true);
    this.Curves = new Array(this.ChannelCount + 1);
    this.selectedIndex = 0;
}

BA.Utils.Extend(PEQGraph, Graph);
//PEQGraph.constructor = PEQGraph;

PEQGraph.prototype.Init = function() {
    //Initialize curve calc values
    PEQGraph.superproto.Init.call(this);
    var sign = 1;
    for (var i = 0; i < this.ChannelCount; i++) {
        this.CurveCalc.settings[i].gainDB = 0;//sign * 20.0f * (i+1) / ChannelCount;
        this.CurveCalc.settings[i].freq = 20000;
        this.CurveCalc.settings[i].Q = 1;
        this.CurveCalc.settings[i].filterType = 0;

        this.Curves[i] = new PIXI.Graphics;
        this.context.AddObject(this.Curves[i]);

        sign *= -1;
    }

    this.Curves[this.ChannelCount] = new PIXI.Graphics;
    this.context.AddObject(this.Curves[this.ChannelCount]);

    //Initialize calculated curve positions array
    this.computedData = new Array(this.HorizontalDivisions+1);
    for(var i = 0; i < this.HorizontalDivisions+1; ++i) {
        this.computedData[i] = new Array(this.ChannelCount + 1);
    }

    this.selectedIndex = 0;
}

PEQGraph.prototype.randomize = function() {
    var me = this;
    for (var i = 0; i < me.ChannelCount; i++) {
        var x = me.nodeToGraphX(Math.random() * me.Width()),
            y = me.nodeToGraphY(Math.random() * me.Height());


        me.CurveCalc.settings[i].freq = x;// / me.context.canvas;
        me.CurveCalc.settings[i].gainDB = y;
        me.context.requestRenderFrame = true;
    }
}

PEQGraph.prototype.Update = function() {
    if (!this.context.requestRenderFrame) {
    } else {
        var color = [
            0x40BB42,
            0x40BB42,
            0x40BB42,
            0x40BB42,

            0x40BB42,
            0x40BB42,
            0x40BB42,
            0x40BB42,

            0x40BB42,
            0x40BB42,
            0x40BB42
        ];

        for (var i = 0; i < this.ChannelCount; i++) {
            this.CurveCalc.computeCoefficients(i);
        }

        //this.HorizontalDivisions = this.context.Width();
        // compute all graph points
        for (var i = 0; i < this.HorizontalDivisions + 1; i++) {
            var v = this.CurveCalc.computeGain(20.0 * Math.pow(10.0, 3.0 * (i / this.HorizontalDivisions)));
            if (v < -1) v = -1;
            else if (v > 1) v = 1;
            this.computedData[i][this.ChannelCount] = 20.0 * v;
            for (var n = 0; n < this.ChannelCount; n++) {
                this.computedData[i][n] = 20.0 * this.CurveCalc.sectionResponses[n];
            }
        }

        /*for (var i = 0; i < this.HorizontalDivisions + 1; i++) {
            var accum = 0.0;
            for (var n = 0; n < this.ChannelCount; n++) {
                accum += this.computedData[i][n];
            }

            this.computedData[i][this.ChannelCount] = accum;// / (double) knobs.length();
        }*/

        for (var i = 0; i < this.ChannelCount; i++) {
           // if (i == this.selectedIndex) continue;
            this._drawHump(i, color[i], color[i], true);
        }

        //this._drawHump(this.selectedIndex, color[this.selectedIndex], color[this.selectedIndex], true);
        //this._drawHump(this.ChannelCount, color[this.ChannelCount], color[this.ChannelCount], false);
    }

    PEQGraph.superproto.Update.call(this);
}

PEQGraph.prototype._drawHump = function(n, color1, color2, showKnob) {
    //if (showKnob) this._drawHumpSolid(n, color1);
    this._drawHumpOutline(n, color2);
    //if (showKnob) _drawKnob(n, color2);
}

PEQGraph.prototype._drawHumpOutline = function(n, color) {
    var me = this,
        left = me.graphToNodeX(me.xAxis.min),
        width = me.graphToNodeX(me.xAxis.max) - me.graphToNodeX(me.xAxis.min),
        stride = width / me.HorizontalDivisions,
        curve = this.Curves[n];

    curve.clear();
    curve.lineStyle(2, 0x40BB42, 1);
    //curve.beginFill(color, 0.3);

    for (var i = 0; i < me.HorizontalDivisions; i++) {
        var x1 = left + (i) * stride,
            y1 = me.graphToNodeY(this.computedData[i][n]);

        if (i == 0) {
            curve.moveTo(x1, y1);
        }
        else {
            curve.lineTo(x1, y1);
        }
    }

    curve.lineTo(this.Width() + 50, y1);
    curve.lineTo(this.Width(), this.Height());
    curve.lineTo(0, this.Height());
    //curve.endFill();
}



