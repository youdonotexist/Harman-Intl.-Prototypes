/**
 * Created by mdaly on 12/19/13.
 */

BlinkySprite2 = function(rendercontext)
{
    this.animationFrames = rendercontext.SheetManager.Animations["redBlinky"];
    this.animationIndex = 0;
    this.elapsedSinceLastFrame = 0.0;
    this.sprite;
}

BlinkySprite2.constructor = BlinkySprite2;

BlinkySprite2.prototype.buildSprite = function(stage, position) {
    // create a texture from an image path
    // create a new Sprite using the texture
    this.sprite = new PIXI.MovieClip(this.animationFrames);

    // center the sprites anchor point
    this.sprite.anchor.x = 0.5;
    this.sprite.anchor.y = 0.5;

    // move the sprite t the center of the screen
    this.sprite.position.x = position.x;
    this.sprite.position.y = position.y;

    this.sprite.animationSpeed = 0.1;

    this.sprite.gotoAndPlay(0);

    stage.addChild( this.sprite);

    return this;
}

BlinkySprite2.prototype.update = function() {

}