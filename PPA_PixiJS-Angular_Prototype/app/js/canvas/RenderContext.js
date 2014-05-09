/**
 * Created by mdaly on 12/27/13.
 */

RenderContext = function(width, height, callback, scope) {
    var me = this;

    // create an new instance of a pixi stage
    me.stage = new PIXI.Stage(0x000000, true);

    // create a renderer instance
    //me.renderer = PIXI.autoDetectRenderer(width, height);
    me.renderer = new PIXI.CanvasRenderer(width, height);

    me.createDate = new Date();
    me.GameTime = new Time();
    me.SheetManager = new SpriteSheetManager();
    me.GameObjectManager = [];

    me.GameContainer = new PIXI.DisplayObjectContainer();
    me.stage.addChild(me.GameContainer);

    me.GameContainer.interactive = true;
    me.GameContainer.mousedown = onTouchDown;
    me.GameContainer.mouseup = onTouchUp;

    me.requestRenderFrame = true;

    if (callback) {
        callback.call(scope);
    }

    me.resize = resize2;
    me.Update = Update;

    $(window).resize(function(){me.requestRenderFrame = true; me.Update();});

    function onTouchDown(interactionData) {
        me.scope.mousedown = interactionData;
    }

    function onTouchUp(interactionData) {
        me.scope.mousedown = false;
    }

    function onAssetsLoaded() {
        var greenAnim = [PIXI.Texture.fromFrame("off.png"), PIXI.Texture.fromFrame("on.png")];
        var redAnim = [PIXI.Texture.fromFrame("danger_off.png"), PIXI.Texture.fromFrame("danger_on.png")];

        me.SheetManager.Animations["greenBlinky"] = greenAnim;
        me.SheetManager.Animations["redBlinky"] = redAnim;

        if (callback) {
            callback.call(scope);
        }
    }

    /**
     * Resizes the canvas object based on the parent element's width/height
     */
    function resize2() {
        var w = me.renderer.view.parentNode.clientWidth;
        var h = me.renderer.view.parentNode.clientHeight;

        if (w != me.lcw || h != me.lch)
        {
            me.renderer.resize(w, h);
            me.GameContainer.hitArea = new PIXI.Rectangle(0, 0, w, h);
        }

        me.lcw = w;
        me.lch = h;
    }


    function Update() {
        resize2();

        me.GameTime.update();

        for (var i = 0; i < me.GameObjectManager.length; i++) {
            var child = me.GameObjectManager[i];
            child.update();
        }

        // render the stage
        me.renderer.render(me.stage);
    }
};

RenderContext.constructor = RenderContext;

RenderContext.prototype.Width = function() { return this.renderer.view.width; }
RenderContext.prototype.Height = function() { return this.renderer.view.height; }
RenderContext.prototype.AddObject = function(obj) { this.GameContainer.addChild(obj); }
RenderContext.prototype.GetDomElement = function() { return this.renderer.view; }
