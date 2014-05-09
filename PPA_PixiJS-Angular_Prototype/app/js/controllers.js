'use strict';

/* Controllers */

var module = angular.module('Blinky.controllers', []);

module.controller('BlinkyController', ['$scope', '$rootScope', function($scope, $rootScope) {
    window._Randomize = function(time) {
        if (time) {
            window.randomTime = time;
        }

        /**
         * START - Debug Code used to generate random eq graphs
         */
        if (!window.randomizeList) {
            window.randomizeList = [];
        }
        for (var i = 0; i < window.randomizeList.length; i++) {
            window.randomizeList[i].randomize();
        }

        setTimeout(window._Randomize, window.randomTime);

        /**
         * END
         */
    }
}]);

module.controller('RunShowController', ['$scope', function($scope){
    //This logic will be replaced when we know the actual number of inputs
    $scope.items = [];
    $scope.index = 10;
    for (var i = 0; i < 10; i++) {
        $scope.items.push(i);
    }

    //Used by the horizontal scrollview to denote if there are more inputs to be rendered
    $scope.more = function() {
        $scope.items.push($scope.index++);
    }
}]);
module.controller('PEQGraphController', ['$scope', function($scope){
    $scope.mouseDown = false;
}]);