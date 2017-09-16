
#include "image.h"

Image::Image() {

	initialized = false;
	spriteData.width = 2;
	spriteData.height = 2;
	spriteData.x = 0.0;
	spriteData.y = 0.0;
	spriteData.scale = 1.0;
	spriteData.angle = 0.0;
	spriteData.rect.left = 0;
	spriteData.rect.top = 0;
	spriteData.rect.right = spriteData.width;
	spriteData.rect.bottom = spriteData.height;
	spriteData.texture = NULL;
	spriteData.flipHorizontal = false;
	spriteData.flipVertical = false;
	cols = 1;
	textureManager = NULL;
	startFrame = 0;
	endFrame = 0;
	currentFrame = 0;
	frameDelay = 1.0f;
	animTimer = 0.0f;
	fadeTimer = 0.0f;
	fadealpha = 0;
	visible = true;
	loop = true;
	animComplete = false;
	fadeComplete = false;
	graphics = NULL;
	colorFilter = graphicsNS::WHITE;
}
	
Image::~Image() {

}

bool Image::initialize(Graphics *g, int width, int height, int ncols, TextureManager *textureM) {

	try {
		graphics = g;
		textureManager = textureM;
		spriteData.texture = textureManager->getTexture();
		if (width == 0) {
			width = textureManager->getWidth();
		}//if
		spriteData.width = width;
		if (height == 0) {
			height = textureManager->getHeight();
		}
		spriteData.height = height;
		cols = ncols;
		if (cols == 0) {
			cols = 1;
		}
		spriteData.rect.left = (currentFrame % cols) * spriteData.width;
		spriteData.rect.right = spriteData.rect.left + spriteData.width;
		spriteData.rect.top = (currentFrame / cols) * spriteData.height;
		spriteData.rect.bottom = spriteData.rect.top + spriteData.height;
	}
	catch (...) {
		return false;
	}
	initialized = true;
	return true;
}

void Image::draw(COLOR_ARGB color) {
	
	if (!visible || graphics == NULL) {
		return;
	}

	spriteData.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER) {
		graphics->drawSprite(spriteData, colorFilter);
	}//if
	else {
		graphics->drawSprite(spriteData, color);
	}//else
}

void Image::draw(SpriteData sd, COLOR_ARGB color) {

	if (!visible || graphics == NULL) {
		return;
	}

	sd.rect = spriteData.rect;
	sd.texture = textureManager->getTexture();
	if (color == graphicsNS::FILTER) {
		graphics->drawSprite(sd, colorFilter);
	}
	else {
		graphics->drawSprite(sd, color);
	}
	fadealpha = 255;
	fadeComplete = true;
}

void Image::update(float frameTime) {

	if (endFrame - startFrame > 0) {
		animTimer += frameTime;
		if (animTimer > frameDelay) {
			animTimer -= frameDelay;
			currentFrame++;
			if (currentFrame < startFrame || currentFrame > endFrame) {
				if (loop == true) {
					currentFrame = startFrame;
				}//loop ==true
				else {
					currentFrame = endFrame;
					animComplete = true;
				}//else
			}//currentFrame < startFrame || currentFrame > endFrame
			setRect();
		}//animTimer > frameDelay
	}//endFrame - startFrame
}

void Image::setCurrentFrame(int c) {
	
	if (c >= 0) {
		currentFrame = c;
		animComplete = false;
		setRect();
	}//if
}

inline void Image::setRect() {

	spriteData.rect.left = (currentFrame % cols) * spriteData.width;
	spriteData.rect.right = spriteData.rect.left + spriteData.width;
	spriteData.rect.top = (currentFrame / cols) * spriteData.height;
	spriteData.rect.bottom = spriteData.rect.top + spriteData.height;

}

//フェードアウト
void Image::fadeOut(float frameTime, float fadeoutTime) {

	if (fadeComplete != false) {
		if (fadeoutTime > 0) {
			if (fadeTimer < fadeoutTime) {
				fadealpha = (int)(255 - 255 * fadeTimer / fadeoutTime);
				draw(D3DCOLOR_ARGB(fadealpha, 255, 255, 255));
				fadeTimer += frameTime;
			}
			else {
				fadealpha = 0;
				fadeTimer = 0.0f;
				fadeComplete = false;
			}
		}//fadeoutTimer >0
	}//fadeComplete != false
}

//フェードイン
void Image::fadeIn(float frameTime, float fadeinTime) {

	if (fadeComplete != true) {
		if (fadeinTime > 0) {
			if (fadeTimer < fadeinTime) {
				fadealpha = (int)(255 * fadeTimer / fadeinTime);
				draw(D3DCOLOR_ARGB(fadealpha, 255, 255, 255));
				fadeTimer += frameTime;
			}
			else {
				fadealpha = 255;
				fadeTimer = 0.0f;
				draw();
				fadeComplete = true;
			}
		}//fadeinTimer > 0
	}//fadeComplete != true
}

