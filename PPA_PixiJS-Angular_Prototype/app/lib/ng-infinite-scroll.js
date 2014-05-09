
/* ng-infinite-scroll - v1.0.0 - 2013-02-23 */
var mod;

mod = angular.module('infinite-scroll', []);

mod.directive('infiniteScrollHorizontal', ['$rootScope', '$window', '$timeout', function($rootScope, $window, $timeout) {
    return {
        link: function(scope, elem, attrs) {
            var checkWhenEnabled, handler, scrollDistance, scrollEnabled;

            $window = angular.element(elem).find('.infinite-scroll-container');
            scrollDistance = 0;
            if (attrs.infiniteScrollDistance != null) {
                scope.$watch(attrs.infiniteScrollDistance, function(value) {
                    return scrollDistance = parseInt(value, 10);
                });
            }
            scrollEnabled = true;
            checkWhenEnabled = false;
            if (attrs.infiniteScrollDisabled != null) {
                scope.$watch(attrs.infiniteScrollDisabled, function(value) {
                    scrollEnabled = !value;
                    if (scrollEnabled && checkWhenEnabled) {
                        checkWhenEnabled = false;
                        return handler();
                    }
                });
            }
            handler = function() {
                var shouldScroll = $window.context.offsetWidth + $window.context.scrollLeft >= ($window.context.scrollWidth - 500);
                if (shouldScroll && scrollEnabled) {
                    if ($rootScope.$$phase) {
                        return scope.$eval(attrs.infiniteScrollHorizontal);
                    } else {
                        return scope.$apply(attrs.infiniteScrollHorizontal);
                    }
                } else if (shouldScroll) {
                    return checkWhenEnabled = true;
                }
            };

            $window.context.onscroll = handler;
            scope.$on('$destroy', function() {
                return $window.off('scroll', handler);
            });
            return $timeout((function() {
                if (attrs.infiniteScrollImmediateCheck) {
                    if (scope.$eval(attrs.infiniteScrollImmediateCheck)) {
                        return handler();
                    }
                } else {
                    return handler();
                }
            }), 0);
        }
    };
}]);
