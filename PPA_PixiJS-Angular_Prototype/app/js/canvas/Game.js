/**
 * Created by mdaly on 12/19/13.
 */

/*
 *  little time class!
 */
Time = function()
{
    this.deltaTime = 1;
    this.lastTime = 0;
}

Time.constructor = Time;

Time.prototype.update = function()
{
    var time = Date.now();
    var currentTime =  time;
    var passedTime = currentTime - this.lastTime;

    if(passedTime > 100)passedTime = 100;

    this.DELTA_TIME = (passedTime * 0.06);
    this.lastTime = currentTime;
}

SpriteSheetManager = function() {
    this.Sheets = {}
    this.Animations = {}
}

SpriteSheetManager.constructor = SpriteSheetManager;