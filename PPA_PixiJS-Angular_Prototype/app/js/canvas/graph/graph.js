/**
 * Created by mdaly on 12/31/13.
 */

Graph = function(width, height) {
    this.ChannelCount = 1;
    this.HorizontalDivisions = 500; // number of quads, left to right

    this.xAxis = new Graph.AxialInfo();
    this.yAxis = new Graph.AxialInfo();

    this.yAxisLabels = {}; // label name to label object
    this.xAxisLabels = {}; // label name to label object

    this.xLabelOffset = 10;
    this.yLabelOffset = 10;

    this.context = new RenderContext(width, height, this.onReady, this);

    this.graphics = new PIXI.Graphics;
    this.context.AddObject(this.graphics);
    this.drawAxes = false;

    var me = this;
    requestAnimationFrame(this.Update.bind(this));
}

Graph.GraphMode = {
    Linear: 0,
    Exponential: 1
}

Graph.AxialInfo =  function() {
    this.min        = 0;
    this.max        = 0;
    this.graphMode  = Graph.GraphMode.Linear;
    this.subminor   = new Array();
    this.minor      = new Array();
    this.major      = new Array();
    this.tick       = new Array();
    this.label      = new Array();
}

Graph.prototype.Init = function() {

}

/**
 * Converts from axis-space to control-space
 * @param graphX
 * @returns {number}
 */

Graph.prototype.graphToNodeX = function (graphX) {
    var norm,
        me = this;

    if (me.xAxis.mode == Graph.GraphMode.Linear) {
        norm = (graphX - me.xAxis.min) / (me.xAxis.max - me.xAxis.min);
    }
    else {
        var expRange =  BA.Utils.Log10(me.xAxis.max / me.xAxis.min);

        norm = BA.Utils.Log10(graphX / me.xAxis.min) / expRange;
    }

    return norm * me.MaxX();
}

/**
 * Converts from axis-space to control-space
 * @param graphX
 * @returns {number}
 */
Graph.prototype.graphToNodeY = function(graphY) {
    var norm,
        me = this;

    if (me.yAxis.mode == Graph.GraphMode.Linear) {
        norm = (graphY - me.yAxis.min) / (me.yAxis.max - me.yAxis.min);
    }
    else {
        var expRange = BA.Utils.Log10(me.yAxis.max / me.yAxis.min);

        norm = BA.Utils.Log10(graphY / me.yAxis.min) / expRange;
    }

    return norm * (me.MaxY());
}

/**
 * Converts from control-space to axis-space
 * @param graphX
 * @returns {number}
 */
Graph.prototype.nodeToGraphX = function(nodeX) {
    var me = this,
        norm = (nodeX) / (me.context.Width());

    if (me.xAxis.mode == Graph.GraphMode.Linear) {
        return norm * (me.xAxis.max - me.xAxis.min) + me.xAxis.min;
    }
    else {
        var expRange = BA.Utils.Log10(me.xAxis.max / me.xAxis.min);

        return Math.pow(10.0, norm * expRange) * me.xAxis.min;
    }
}

/**
 * Converts from control-space to axis-space
 * @param graphX
 * @returns {number}
 */
Graph.prototype.nodeToGraphY = function(nodeY) {
    var me = this,
        norm = (nodeY) / (me.context.Height());

    if (me.yAxis.mode == Graph.GraphMode.Linear) {
        return norm * (me.yAxis.max - me.yAxis.min) + me.yAxis.min;
    }
    else {
        var expRange = BA.Utils.Log10(me.yAxis.max / me.yAxis.min);

        return Math.pow(10.0, norm * expRange) * me.yAxis.min;
    }
}

/**
 * Converts from axis-space to control-space
 * @param graphX
 * @returns {number}
 */
Graph.prototype.graphToNode = function(graph) {
    return new PIXI.Point(this.graphToNodeX(graph.x), this.graphToNodeY(graph.y));
}

/**
 * Converts from control-space to axis-space
 * @param graphX
 * @returns {number}
 */
Graph.prototype.nodeToGraph = function(node) {
    return new PIXI.Point(this.nodeToGraphX(node.x), this.nodeToGraphY(node.y));
}

Graph.prototype._drawLine = function(p1, p2, width) {
    this.graphics.moveTo(p1.x, p1.y);
    this.graphics.lineTo(p2.x, p2.y);
}

/**
 * returns x minimum in control-space
 * @returns {number}
 * @constructor
 */
Graph.prototype.MinX  = function() {
    return 0.0;
}

/**
 * returns x minimum in control-space
 * @returns {number}
 * @constructor
 */
Graph.prototype.MinY = function() {
    return 0.0;
}

/**
 * Returns y maximum in control-space
 * @returns {*}
 * @constructor
 */
Graph.prototype.MaxY  = function() {
    return this.context.Height();
}

/**
 * Returns x maximum in control-space
 * @returns {*}
 * @constructor
 */
Graph.prototype.MaxX = function() {
     return this.context.Width();
}

Graph.prototype.Width = function() {
    return this.context.Width();
}

Graph.prototype.Height = function() {
    return this.context.Height();
}

Graph.prototype.DrawXAxis = function() {
    var majors = this.xAxis.major,
        y1 = this.MaxY(),
        y2 = this.MinY();
    for (var i = 0; i < majors.length; i++) {
        var x = this.graphToNodeX(majors[i]);

        this._drawLine(new PIXI.Point(x , y1), new PIXI.Point(x, y2), 3.0);
    }

    var minors = this.xAxis.minor;
    for (var i = 0; i < minors.length; i++) {
        var x = this.graphToNodeX(minors[i]);

        this._drawLine(new PIXI.Point(x,y1), new PIXI.Point(x, y2), 1.0);
    }
}

Graph.prototype.DrawYAxis = function() {
    var majors = this.yAxis.major,
        x1 = this.MinX(),
        x2 = this.MaxX();

    // Y AXIS FEATURES
    for (var i = 0; i < majors.length; i++) {
        var y = this.graphToNodeY(majors[i]);

        this._drawLine(new PIXI.Point(x1, y), new PIXI.Point(x2, y), 3);
    }

    var minors = this.yAxis.minor;
    for (var i = 0; i < minors.length; i++) {
        var y = this.graphToNodeY(minors[i]);

        this._drawLine(new PIXI.Point(x1, y), new PIXI.Point(x2, y), 1);
    }
}

Graph.prototype.Update = function() {
    var me = this;

    if (me.context.requestRenderFrame) {

        if (me.drawAxes) {
            me.graphics.clear();
            me.graphics.lineStyle (1.5, 0x1F5C20, 1);
            me.DrawXAxis();
            me.DrawYAxis();
        }

        me.context.Update();
        me.context.requestRenderFrame = false;
    }

    //Keep asking for updates
    requestAnimationFrame(function() { me.Update(); } );
}

Graph.prototype.resize = function() { this.context.resize(); }
Graph.prototype.GetDomElement = function() { return this.context.GetDomElement(); }
