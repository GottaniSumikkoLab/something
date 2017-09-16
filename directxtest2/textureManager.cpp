//textureManager.cpp

#include"textureManager.h"

TextureManager::TextureManager() {

	texture = NULL;
	width = 0;
	height = 0;
	file = NULL;
	graphics = NULL;
	initialized = false;

}

TextureManager::~TextureManager() {
	SAFE_RELEASE(texture);
}

bool TextureManager::initialize(Graphics *g, const char *f) {

	try {
		graphics = g;
		file = f;

		hr = graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
		if (FAILED(hr)) {
			SAFE_RELEASE(texture);
			return false;
		}//if
	}//try
	catch (...) {
		return false;
	}
	initialized = true;//³í‚É‰Šú‰»‚³‚ê‚½ê‡Atrue‚ðÝ’è
	return true;
}

void TextureManager::onLostDevice() {
	
	if (!initialized) {
		return;
	}
	SAFE_RELEASE(texture);
}

void TextureManager::onResetDevice() {
	if (!initialized) {
		return;
	}
	graphics->loadTexture(file, TRANSCOLOR, width, height, texture);
}
