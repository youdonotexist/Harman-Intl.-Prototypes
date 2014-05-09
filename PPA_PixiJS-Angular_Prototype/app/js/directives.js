'use strict';

/* Directives */


var dir_module = angular.module('Blinky.directives', []);

/**
 * Generates the HTML to render a vertical slider
 */
dir_module.directive('verticalSlider', function() {
    return function (scope, element) {
       element.noUiSlider({
            range: [0, 100]
            ,start: 40
            ,step: 1
            ,handles: 1
            ,margin: 20
            //,connect: true
            //,direction: 'rtl'
            ,orientation: 'vertical'
            ,behaviour: 'tap-drag'
            ,serialization: {
                mark: ','
                ,resolution: 0.1
                ,to: undefined
            }
        });
    }
})

/**
 * Generates the PixiJS canvas for canvas-based rendering
 */
dir_module.directive('peqGraph', function() {
    return function(scope, iElement) {
        var peqGraph = new PEQGraph(iElement.width(), iElement.height());

        var x = peqGraph.xAxis;
        var y = peqGraph.yAxis;

        // PEQ Graph
        x.mode = Graph.GraphMode.Exponential;
        x.min = 20;
        x.max = 20000;
        x.major.push(20);
        x.minor.push(40);
        x.minor.push(80);
        x.minor.push(200);
        x.minor.push(500);
        x.minor.push(1000);
        x.minor.push(2000);
        x.minor.push(5000);
        x.minor.push(10000);
        x.minor.push(20000);
        x.tick.push(20);
        x.tick.push(40);
        x.tick.push(80);
        x.tick.push(200);
        x.tick.push(500);
        x.tick.push(1000);
        x.tick.push(2000);
        x.tick.push(5000);
        x.tick.push(10000);
        x.tick.push(20000);

        y.mode = Graph.GraphMode.Linear;
        y.min = -20;
        y.max = 20;
        y.major.push(-20);
        y.minor.push(-12);
        y.minor.push(-6);
        y.major.push(0);
        y.minor.push(6);
        y.minor.push(12);
        y.minor.push(20);
        y.tick.push(-20);
        y.tick.push(-12);
        y.tick.push(-6);
        y.tick.push(0);
        y.tick.push(6);
        y.tick.push(12);
        y.tick.push(20);

        scope.peqGraph = peqGraph;
        scope.peqGraph.scope = scope;
        scope.peqGraph.context.scope = scope;
        iElement.append(peqGraph.GetDomElement());

        scope.peqGraph.Init();

        if (!window.randomizeList) {
            window.randomizeList = [];
        }

        window.randomizeList.push(peqGraph);
    }
});


