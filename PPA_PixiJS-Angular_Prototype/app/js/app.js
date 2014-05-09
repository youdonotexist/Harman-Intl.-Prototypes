'use strict';


// Declare app level module which depends on filters, and services
angular.module('Blinky', [
  'ionic',
  'ngRoute',
   'infinite-scroll',
  'Blinky.filters',
  'Blinky.services',
  'Blinky.directives',
  'Blinky.controllers'
]).
config(['$routeProvider', function($routeProvider) {
  $routeProvider.when('/blinkycanvas', {templateUrl: 'partials/CanvasTest.html', controller: 'BlinkyController'});
  $routeProvider.otherwise({redirectTo: '/blinkycanvas'});
}]);
