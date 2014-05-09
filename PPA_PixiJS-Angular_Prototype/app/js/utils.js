/**
 * Created by mdaly on 12/31/13.
 */

BA = {}
BA.Utils = {}
BA.Utils.Extend = function(subclass, superclass) {
    function Dummy() {}
    Dummy.prototype = superclass.prototype;
    subclass.prototype = new Dummy();
    subclass.prototype.constructor = subclass;
    subclass.superclass = superclass;
    subclass.superproto = superclass.prototype;
}

BA.Utils.Log10 = function(value) {
        // From: http://phpjs.org/functions
        // +   original by: Philip Peterson
        // +   improved by: Onno Marsman
        // +   improved by: Tod Gentille
        // +   improved by: Brett Zamir (http://brett-zamir.me)
        // *     example 1: log10(10);
        // *     returns 1: 1
        // *     example 2: log10(1);
        // *     returns 2: 0
        return Math.log(value) / 2.302585092994046; // Math.LN10
}